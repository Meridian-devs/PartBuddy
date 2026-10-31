#pragma once

#include <string>
#include <sqlite3.h>
#include <Component.hpp>

/*
    @class Database
    @brief Manages SQLite database connection and operations.

    * Responsible for establishing and managing the database connection,
    * initializing the database, and preforming operations on component records.
    * 
    * The database class seperates database-specific functionality from inventory
    * management and the CLI.
*/

class Database
{
public:
    explicit Database(const std::string& databasePath);     // Database Contructor
    ~Database();                                            // Database Destructor

    bool open();                                            // Method: Open connection to SQLite
    void close();                                           // Method: Close connection to SQLite
    bool initialize();                                      // Method: Initialize the database schema

/*
    @brief Inserts a component into the database
    
    * Uses a prepared SQL statement to inset the supplied component info into the components table

    @param component reference to the Component being inserted.
    @return true if the insertion succeeds, false otherwise.
*/

    bool addComponent(const Component& component);

    sqlite3* getConnection() const;                         // Returns the underlying SQLite connection, pointer to active SQLite database connection

private:
    std::string databasePath;                               // Filesystem path to SQLite database
    sqlite3* connection = nullptr;                          // Active SQLite database connection
};