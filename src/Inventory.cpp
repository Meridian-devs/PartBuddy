#include "Inventory.hpp"
#include "Database.hpp"

/**
 * @brief Construccts an Inventory object
 * 
 * Stores a reference to the supplied Database instance.
 * The inventory object does not create or own the database
 * 
 * @param database Existing Database instance
 */

Inventory::Inventory(Database& database)
    : database(database)
{
}

/**
 * @brief Adds a component to the inventory
 * 
 * Passes the component to the Database layer, which is responsible
 * for inserting it into SQLite
 * 
 * @param component Component information to add
 * @return true if the database insertion succeeds, false otherwise.
 */

bool Inventory::addComponent(const Component& component)
{
    return database.addComponent(component);
}

/**
 * @brief Removes a component from the inventory.
 *
 * Delegates the removal operation to the Database layer.
 *
 * @param partNumber Part number identifying the component.
 * @return true if the component was removed, false otherwise.
 */
bool Inventory::removeComponent(const std::string& partNumber)
{
    return database.removeComponent(partNumber);
}

/**
 * @brief Lists the component inventory
 * 
 * @param
 * @return
 */

//bool Inventory::listComponents()