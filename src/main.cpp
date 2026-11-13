#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>

#include <Database.hpp>
#include <Inventory.hpp>
#include <Component.hpp>

/**
 * @brief Displays the PartBuddy welcome graphic and message.
 * 
 * Prints the appliccation banner and instructions for accessing the command list.
 */

void printWelcome()
{
    std::cout << R"(
                 .*%%%-..                           
            .:*#%%%%%%%%%%-..                      
           .#@%%%%%%%%%%%%%%%#-..           888888ba                      dP    888888ba                 dP       dP
           :@@@@@@%%%%%%%%%%%%%%%*.         88    `8b                     88    88    `8b                88       88
           .-.*@%@@@@%%%%%%%%%%@@@#.        a88aaaa8P' .d8888b. 88d888b. d8888P a88aaaa8P'dP    dP .d888b88 .d888b88 dP    dP
            =+# .=%%@@@@%%%%@@@@@@%.        88        88'  `88 88'  `88   88    88   `8b. 88    88 88'  `88 88'  `88 88    88
            :=. +*=:.%@@@@@@@@@@@%.         88        88.  .88 88         88    88    .88 88.  .88 88.  .88 88.  .88 88.  .88
            .=  =.=++%:.=%@@@@@*+:          dP        `88888P8 dP         dP    8888888P  `88888P' `88888P8 `88888P8 `8888P88
            .+. = .=:.++#  @%% .=.                                                                                        .88
            ... =  = .-+.      ...                                                                                    d8888P
                .  -  ::                        
                   .  ::                                An electronics inventory manager built for the bench.
                      ..
)";

    std::cout << "\n\n" << "Type 'help' for a list of available commands." << "\n\n";
}

void printHelp()    // Displays the available commands
{
    std::cout << R"(
    Available Commands:
        add     Add a component
        remove  Remove a component
        list    List all components
        stock   Adjust component quantity

        help    Display this menu
        exit    Close PartBuddy

        ** Not yet implemented... **
        search  Search for components
        edit    Edit a component
)";
}

/**
 * @brief Collects component information and adds it to the inventory
 * 
 * Prompts the user for component information, quantity, and physical location.
 * Component information will be auto-filled when web-scraping and datasheet parsing is
 * implemented (this is a minimal working version).
 * 
 * Retrieves a list of added components to check for duplicated additions.
 * 
 * @param inventory Inventory instance used to add the component
 * @return true if the component is added successfully, false otherwise.
 */

bool handleAdd(Inventory& inventory)
{
    Component component;
    std::vector<Component> components = inventory.getComponents();

    std::cout << "\n--- Add Component ---\n";

    std::cout << "Part number: ";
    if (!std::getline(std::cin, component.partNumber))
        return false;

    if (component.partNumber.empty())
    {
        std::cout << "Part number cannot be empty.\n";
        return false;
    }

    // Search through each component for duplicate part numbers.
    for (const Component& existingComponent : components)
    {
        if (existingComponent.partNumber == component.partNumber) 
        {
            std::cout << "Part number already exists.\n";
            return false;
        }
    }

    std::cout << "Manufacturer: ";
    if (!std::getline(std::cin, component.manufacturer))
        return false;

    std::cout << "Description: ";
    if (!std::getline(std::cin, component.description))
        return false;

    std::cout << "Category: ";
    if (!std::getline(std::cin, component.category))
        return false;

    std::cout << "Package: ";
    if (!std::getline(std::cin, component.package))
        return false;

    std::cout << "Datasheet URL: ";
    if (!std::getline(std::cin, component.datasheetURL))
        return false;

    std::cout << "Quantity: ";

    std::string quantityInput;
    if (!std::getline(std::cin, quantityInput))
        return false;

    try
    {
        // Track how many characters were used by std::stoi
        // Allows us to reject input such as "12abc".
        std::size_t charactersRead = 0;

        int quantity = std::stoi(quantityInput, &charactersRead);

        if (charactersRead != quantityInput.size() || quantity < 0)
        {
            std::cout << "Quantity must be a non-negative integer.\n";
            return false;
        }
        component.quantity = quantity;
    }
    catch (const std::exception&)
    {
        // std::stoi throws if the input cannot be converted to an integer or is outside the supported range.
        std::cout << "Invalid quantity. Enter a whole number.\n";
        return false;
    }

    std::cout << "Location: ";
    if (!std::getline(std::cin, component.location))
        return false;

    // Pass the completed Component object to the Inventory layer.
    if (inventory.addComponent(component))
    {
        std::cout << "Component added successfully!\n";
        return true;
    }

    std::cout << "Failed to add component.\n";
    return false;
}

/**
 * @brief Prompts the user to remove a component
 * 
 * Collects the component's part number and passes it to the inventory layer for removal.
 * Confirms with the user if this action should be preformed.
 * 
 * @param inventory Inventory instance used for removal.
 * @return true if the component was removed successfully, false otherwise.
 */

bool handleRemove(Inventory& inventory)
{
    std::string partNumber;
    std::string confirmation;

    std::cout << "\n--- Remove Component ---\n";

    std::cout << "Part Number: ";
    if (!std::getline(std::cin, partNumber))
    {
        return false;
    }
        
    if (partNumber.empty())
    {
        std::cout << "Part number cannot be empty.\n";
        return false;
    }
    
    // Added handling to ensure removal is wanted
    std::cout << "Are you sure you want to remove " << partNumber << " ? [Y/N]: ";
    if (!std::getline(std::cin, confirmation))
    {
        return false;
    }

    if (confirmation != "Y" && confirmation != "y")
    {
        std::cout << "Remove cancelled.\n";
        return false;
    }

    if (inventory.removeComponent(partNumber))
    {
        std::cout << "Component removed successfully!\n";
        return true;
    }

    std::cout << "Component not found or failed to remove.\n";
    return false;
}


/**
 * @brief Displays all components currently in the inventory.
 *
 * @param inventory Inventory instance used to retrieve components.
 */

void handleList(Inventory& inventory)
{
    std::vector<Component> components = inventory.getComponents();

    std::cout << "\n\t\t\t--- Component Inventory ---\n\n";

    // Check whether the inventory contains any components.
    if (components.empty())
    {
        std::cout << "Your inventory is empty.\n";
        return;
    }

    // Display the table headings.
    std::cout << std::left
              << std::setw(18) << "Part Number"
              << std::setw(20) << "Manufacturer"
              << std::setw(15) << "Category"
              << std::setw(10) << "Quantity"
              << "Location\n";

    std::cout << std::string(80, '-') << '\n';

    // Display each component.
    for (const Component& component : components)
    {
        std::cout << std::left
                  << std::setw(18) << component.partNumber
                  << std::setw(20) << component.manufacturer
                  << std::setw(15) << component.category
                  << std::setw(10) << component.quantity
                  << component.location << "\n";
    }
}

/**
 * @brief Updates quantity of components currently in the inventory.
 * 
 * Checks if the part requested is in the inventory. (Prompt for add if not?)
 *
 * @param inventory Inventory instance used to retrieve components.
 */

bool handleStock(Inventory& inventory)
{
    std::string partNumber;
    std::string confirmation;
    std::vector<Component> components = inventory.getComponents();

    std::cout << "\n--- Update Component Quantity---\n";

    std::cout << "Part Number: ";
    if (!std::getline(std::cin, partNumber))
    {
        return false;
    }
        
    if (partNumber.empty())
    {
        std::cout << "Part number cannot be empty.\n";
        return false;
    }

    // Check if part is in inventory
    bool part_found = false;

    // Search through each component.
    for (const Component& component : components)
    {
        if (partNumber == component.partNumber) 
        {
            std::cout << "Current Quantity: " << component.quantity << "\n";
            part_found = true;
            break;
        }
    }

    if (part_found == false) 
    {
        std::cout << "Part number cannot be found.\n";
        return false;
    }

    int quantity;
    std::cout << "Quantity: ";

    std::string quantityInput;
    if (!std::getline(std::cin, quantityInput))
        return false;

    try
    {
        // Track how many characters were used by std::stoi
        // Allows us to reject input such as "12abc".
        std::size_t charactersRead = 0;

        quantity = std::stoi(quantityInput, &charactersRead);

        if (charactersRead != quantityInput.size() || quantity < 0)
        {
            std::cout << "Quantity must be a non-negative integer.\n";
            return false;
        }
    }
    catch (const std::exception&)
    {
        // std::stoi throws if the input cannot be converted to an integer or is outside the supported range.
        std::cout << "Invalid quantity. Enter a whole number.\n";
        return false;
    }

    // Added handling to ensure quantity is entered correctly
    std::cout << "Update Quantity for " << partNumber << "? [Y/N]: ";
    if (!std::getline(std::cin, confirmation))
    {
        return false;
    }

    if (confirmation != "Y" && confirmation != "y")
    {
        std::cout << "Quantity update cancelled.\n";
        return false;
    }

    if (inventory.updateQuantity(partNumber, quantity))
    {
        std::cout << "Component quantity updated successfully!\n";
        return true;
    }

    std::cout << "Component not found or failed to update quantity.\n";
    return false;
}

/**
 * @brief Routes a command to its corresponding handler
 * 
 * The CLI is responsible for interpreting user commands.
 * Inventory operations are delegated to the inventory class.
 * 
 * @param command Command entered by the user.
 * @param inventory Inventory instance used by command handlers
 */

void handleCommand(const std::string& command, Inventory& inventory)
{
    if (command == "help") {
        printHelp();                                        // Prints list of commands
    }
    else if (command == "add") {
        handleAdd(inventory);
    }
    else if (command == "list") {
        handleList(inventory);                              // Future: specific component type
    }
    else if (command == "search") {
        std::cout << "Not yet implemented...\n";            // Implement component search
    }
    else if (command == "edit") {
        std::cout << "Not yet implemented...\n";            // Implement edit component details
    }
    else if (command == "stock") {
        handleStock(inventory);
    }
    else if (command == "remove") {
        handleRemove(inventory);
    }
    else if (command.empty()) {
        // Ignore empty input
    }
    else {
        std::cout << "Unknown Command: " << command << "\n";
        std::cout << "Type Help for available commands.\n";
    }
}

/**
 * @brief Starts the PartBuddy tool
 * 
 * Displays welcome message, opens and initializes the database, constructs the Inventory instance,
 * and enters the command-processing loop.
 * 
 * Loop continues until user enters "exit" or "quit", or input stream receives "EOF"
 * 
 * @return 0 if the tool exits normally, a non-zero value if init. fails or input fails
 */

int main()
{
    printWelcome();                                         // Print welcome graphic and message
    std::string command;                                    // Init the command variable

    // Create the database object using the local database file
    Database database("inventory.db");

    // Open the SQLite connection before performing database work
    if (!database.open())
    {
        return 1;
    }

    // Ensure the required database table exists.
    if (!database.initialize())
    {
        return 1;
    }

    // Inventory uses the existing database instance
    Inventory inventory(database);
    std::cout << "Database initialized successfully.\n";

    // Main command-processing loop
    while (true)
    {
        std::cout << "PartBuddy > ";
        if (!std::getline(std::cin, command)) {
            if (std::cin.eof()) {                           // Handle EOF command (CTRL+Z on Win/CTRL+D on Linux-like systems)
                break;
            }

            if (std::cin.bad()) {
                std::cerr << "\nAn input error occurred.\n";
                return -1;
            }

            std::cin.clear();
            continue;
        }

        if (command == "exit" || command == "quit") {
            break;                                          // Handle exit/quit command
        }

        handleCommand(command, inventory);
    }
    std::cout << "\nGoodbye!\n";                            // If exit requested, print goodbye message
    return 0;
}

/*
Configure the project:
    - cmake -S . -B build -G Ninja (Should be one-time operation if 'build' folder exists)

Build the project:
    - cmake --build build
*/