#pragma once

#include <string>

struct Component
{
    // Component specific information
    std::string manufacturer;
    std::string partNumber;
    std::string description;
    std::string category;
    std::string package;
    std::string datasheetURL;

    // Local Inventory Information
    int id = 0;
    int quantity = 0;
    std::string location;
};