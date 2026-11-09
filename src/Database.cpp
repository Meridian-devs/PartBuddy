#include <iostream>

#include "Database.hpp"

/**
 * @brief Constructs a Database object
 * 
 * Stores the path to the SQLite database file. The connection
 * is not opened until open() is called.
 * 
 * @param databasePath Filesystem path to the database file
 */

Database::Database(const std::string& databasePath)
    : databasePath(databasePath)
{
}

/**
 * @brief Destroys the Database object
 * 
 * Automatically closes the database connection to ensure resourcces
 * are relesaed when the object goes out of scope.
 */

Database::~Database()
{
    close();
}

bool Database::open()
{
    if (connection != nullptr)
    {
        return true;
    }

    const int result = sqlite3_open(databasePath.c_str(), &connection);

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(connection) << '\n';

        close();
        return false;
    }

    return true;
}

void Database::close()
{
    if (connection != nullptr)
    {
        sqlite3_close(connection);
        connection = nullptr;
    }
}

bool Database::initialize()
{
    if (connection == nullptr)
    {
        std::cerr << "Database is not open.\n";
        return false;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS components (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            manufacturer TEXT,
            part_number TEXT NOT NULL,
            description TEXT,
            category TEXT,
            package TEXT,
            datasheet_url TEXT,
            quantity INTEGER NOT NULL DEFAULT 0
                CHECK (quantity >= 0),
            location TEXT
        );
    )";

    char* errorMessage = nullptr;

    const int result = sqlite3_exec(
        connection,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to initialize database: "
                  << errorMessage << '\n';

        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool Database::addComponent(const Component& component)
{
    const char* sql = R"(
        INSERT INTO components (
            manufacturer,
            part_number,
            description,
            category,
            package,
            datasheet_url,
            quantity,
            location
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        connection,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: "
                  << sqlite3_errmsg(connection) << '\n';

        return false;
    }

    sqlite3_bind_text(
        statement, 1, component.manufacturer.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement, 2, component.partNumber.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement, 3, component.description.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement, 4, component.category.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement, 5, component.package.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement, 6, component.datasheetURL.c_str(),
        -1, SQLITE_TRANSIENT
    );

    sqlite3_bind_int(statement, 7, component.quantity);

    sqlite3_bind_text(
        statement, 8, component.location.c_str(),
        -1, SQLITE_TRANSIENT
    );

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE)
    {
        std::cerr << "Failed to insert component: "
                  << sqlite3_errmsg(connection) << '\n';

        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);

    return true;
}

bool Database::removeComponent(const std::string& partNumber)
{
    // Ensure the database connection is active.
    if (connection == nullptr)
    {
        std::cerr << "Database is not open.\n";
        return false;
    }

    const char* sql = R"(
        DELETE FROM components 
        WHERE part_number = ?;
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        connection,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare delete: "
                  << sqlite3_errmsg(connection) << '\n';
        return false;
    }

    // Bind the supplied partNumber to the SQL placeholder
    result = sqlite3_bind_text(
        statement,
        1,
        partNumber.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to bind part number: " << sqlite3_errmsg(connection) << "\n";
        sqlite3_finalize(statement);
        return false;
    }

    // Execute DELETE statement
    result = sqlite3_step(statement);

    if (result != SQLITE_DONE)
    {
        std::cerr << "Failed to remove component: "
                  << sqlite3_errmsg(connection) << '\n';

        sqlite3_finalize(statement);
        return false;
    }

    // Check how many rows affected
    const int rowsAffected = sqlite3_changes(connection);

    // Release the prepared statement
    sqlite3_finalize(statement);

    return rowsAffected > 0;
}

/**
 * @brief Retrieves all components from the database
 * 
 * Executes a SELECT query and converts returned rows into an Component obj.
 * 
 * @return A vector containing all retrieved components
 */

std::vector<Component> Database::getComponents() const
{
    std::vector<Component> components;

    if (connection == nullptr)
    {
        std::cerr << "Database is not open.\n";
        return components;
    }

    const char* sql = R"(
        SELECT id, 
        manufacturer, 
        part_number, 
        description, 
        category, 
        package, 
        datasheet_url, 
        quantity, 
        location
        FROM components;
        )";

    sqlite3_stmt* statement = nullptr;

    // Prepare SQL statement
    int result = sqlite3_prepare_v2(
        connection,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to retrieve components: " << sqlite3_errmsg(connection) << "\n";
        return components;
    }

    // Read each row returned by query
    while ((result = sqlite3_step(statement)) == SQLITE_ROW)
    {
        Component component;

        // Retrive integer columns
        component.id = sqlite3_column_int(statement, 0);
        component.quantity = sqlite3_column_int(statement, 7);

        // Retrieve text columns, may return NULL for columns containing NULL
        auto getText = [&](int column) -> std::string
        {
            const unsigned char* value = sqlite3_column_text(statement, column);
            return value ? reinterpret_cast<const char*>(value) : "";
        };

        component.manufacturer = getText(1);
        component.partNumber = getText(2);
        component.description = getText(3);
        component.category = getText(4);
        component.package = getText(5);
        component.datasheetURL = getText(6);
        component.location = getText(8);

        // Add the completed component to the result vector
        components.push_back(component);
    }

    // Check whether the query finished successfully
    if (result != SQLITE_DONE)
    {
        std::cerr << "Error retrieving components: " << sqlite3_errmsg(connection) << "\n";
        components.clear();
    }

    // Released the prepared statement
    sqlite3_finalize(statement);
    return components;
}

sqlite3* Database::getConnection() const
{
    return connection;
}