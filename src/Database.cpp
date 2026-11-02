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

sqlite3* Database::getConnection() const
{
    return connection;
}