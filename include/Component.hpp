#pragma once

#include <string>

/*
    @struct Component
    @brief  Represents an electronic component stored in the inventory

    * Contains both component-specific information and local inventory information.
    * This struct is used to pass component data between the CLI, inventory and database layers.
*/

struct Component
{
    // Component specific information
    std::string manufacturer;           // Part Name
    std::string partNumber;             // Part Number
    std::string description;            // Part Description, functionality
    std::string category;               // Part type (e.g. resistor, capacitor, etc.)
    std::string package;                // Package type (e.g. TO-220, DIP-8)
    std::string datasheetURL;           // URL to Datasheet

    // Local Inventory Information
    int id = 0;                         // Unique database identifier
    int quantity = 0;                   // Number of units in inventory
    std::string location;               // Physical location in inventory
};