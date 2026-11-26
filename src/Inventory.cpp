#include "Inventory.hpp"
#include "Database.hpp"
#include <cctype>

/**
 * @brief Constructs an Inventory object
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
 * @brief Retrieves all components in the inventory
 * 
 * @return A vector containing all inventory components
 */

std::vector<Component> Inventory::getComponents() const
{
    return database.getComponents();
}

/**
* @brief Updates the quantity of a component
* 
* @param partNumber Manufacturer's part number identifying the component to remove.
* @param quantity   New quantity for the component
* @return true if the quantity was updated successfully, false otherwise
*/

bool Inventory::updateQuantity(const std::string& partNumber, int quantity)
{
    return database.updateQuantity(partNumber, quantity);
}

/**
* @brief Updates the details of component. This function specifically handles the string edits.
*
* Delegates the editing operation to the Database layer
* 
* @param partNumber Manufacturer's part number identifying the component to edit.
* @param edit The change that is being made.
* @return true if the change was updated successfully, false otherwise.
*/
bool Inventory::editComponent(const std::string& partNumber, std::string& editField, std::string& editString)
{
   return database.editComponent(partNumber, editField, editString); 
}

/**
* @brief Updates the details of component. This function specifically handles the integer edits.
*
* Delegates the editing operation to the Database layer
* 
* @param partNumber Manufacturer's part number identifying the component to edit.
* @param edit The change that is being made.
* @return true if the change was updated successfully, false otherwise.
*/
bool Inventory::editComponent(const std::string& partNumber, std::string& editField, int editInteger)
{
    return database.editComponent(partNumber, editField, editInteger);
}

/**
* @brief Converts strings to lowercase
* 
* @param s the string to convert to lowercase
* @return a string in all lowercase
*/
std::string Inventory::toLower(std::string s)
{
    for (char& c : s)
    {
        c = std::tolower(c);
    }
    return s;
}

/**
 * @brief Searches for a specific component in the inventory
 * 
 * @param query Manufacturer's part number to locate the component
 * @return A vector containing the components that closely match 
 */

std::vector<Component> Inventory::searchComponent(const std::string& query) const
{
    return database.searchComponent(query);
}