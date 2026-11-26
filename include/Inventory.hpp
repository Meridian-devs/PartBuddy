#pragma once

#include "Component.hpp"

#include <string>
#include <vector>
#include <Database.hpp>

/**
 * @class Inventory
 * @brief Provides operations for managing the component inventory
 * 
 * Acts as an intermediate between the CLI and the database layer.
 * Inventory operations are exposed through this class, while database-specific
 * functionality is handled by Database.
 * 
 * The inventory class does not own the database object. It stores a reference (using
 * Database& database) to an existing database instance.
 */

class Inventory
{
public:
    /**
     * @brief Constructs a database Inventory Object
     * 
     * @param database Reference to the Database instance used for
     *                 inventory persistence. 
     */
    explicit Inventory(Database& database);


    /**
     * @brief Adds a component to the inventory
     * 
     * Delegates the insertion operation to the Database class.
     * 
     * @param component Component information to add
     * @return true is the component is added successfully, false otherwise
     */
    bool addComponent(const Component& component);

    /**
     * @brief Removes a component from the inventory
     * 
     * @param partNumber Manufacturer's part number identifying the component to remove.
     * @return true if the component removal was successful, false otherwise.
     */
    bool removeComponent(const std::string& partNumber);

    /**
     * @brief Updates the quantity of a component
     * 
     * @param partNumber Manufacturer's part number identifying the component to remove.
     * @param quantity   New quantity for the component
     * @return true if the quantity was updated successfully, false otherwise
     */
    bool updateQuantity(const std::string& partNumber, int quantity);

    /**
     * @brief Retrieves all components in the inventory
     * 
     * @return A vector containing the components in the inventory
     */
    std::vector<Component> getComponents() const;

    /**
     * @brief Searches the inventory for matching components.
     * 
     * @param query Search term used to identify matching components.
     * @return A vector containing components matching the query.
     */
    std::vector<Component> searchComponent(const std::string& query) const;

    /**
     * @brief Updates the details of component. This function specifically handles the string edits.
     * 
     * @param partNumber Manufacturer's part number identifying the component to edit.
     * @param editField The field to be changed.
     * @param editValue The value to populate the field being changed.
     * @return true if the change was updated successfully, false otherwise.
     */
    bool editComponent(const std::string& partNumber, const std::string& editField, const std::string& editValue);

    /**
     * @brief Converts strings to lowercase
     * 
     * @param s the string to convert to lowercase
     * @return a string in all lowercase
     */
    static std::string toLower(std::string s);

private:
    Database& database;         // Reference to the database used by this inventory
};