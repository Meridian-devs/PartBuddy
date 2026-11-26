#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>
#include <Component.hpp>

/** 
 * @class Database
 * @brief Manages SQLite database connection and operations.
 * 
 * Responsible for establishing and managing the database connection,
 * initializing the database, and preforming operations on component records.
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

/**
 * @brief Inserts a component into the database
 * 
 * Uses a prepared SQL statement to insert the supplied component info into the components table
 * 
 * @param component reference to the Component being inserted.
 * @return true if the insertion succeeds, false otherwise.
 */

    bool addComponent(const Component& component);

/**
 * @brief Updates component quantity in the database
 * 
 * Updates the quantity of a component using the manufacturer part number.
 * 
 * @param partNumber Part number of the component to update
 * @param quantity   New quantity for the component
 * @return true if component quantity was updated successfully, false otherwise.
 */

    bool updateQuantity(const std::string& partNumber, int quantity);

/**
 * @brief Removes a component from the database
 * 
 * Identifies the component using the manufacturer part number.
 * 
 * @param partNumber Part number of the component to remove
 * @return true if a component was removed, false otherwise.
 */

    bool removeComponent(const std::string& partNumber);

/**
 * @brief Lists the components added to the inventory database
 * @return list of compoenents, emppty if none added.
 */

    std::vector<Component> getComponents() const;

/**
 * @brief Seaches for a specific component in the database based on partnumber
 * @return list of components that match the search term
 */

    std::vector<Component> searchComponent(const std::string& query);

/**
 * @brief Updates the details of component. This function specifically handles the string edits.
 * 
 * @param partNumber Manufacturer's part number identifying the component to edit.
 * @param editField The component field that is undergoing change.
 * @param editValue The value to populate the specified field.
 * @return true if the change was updated successfully, false otherwise.
 */

    bool editComponent(const std::string& partNumber, const std::string& editField, const std::string& editValue);
    
    sqlite3* getConnection() const;                         // Returns the underlying SQLite connection, pointer to active SQLite database connection

private:
    std::string databasePath;                               // Filesystem path to SQLite database
    sqlite3* connection = nullptr;                          // Active SQLite database connection
};