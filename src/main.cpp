#include <iostream>
#include <string>
#include <unordered_map>

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
        list    List all components
        search  Search for components
        edit    Edit a component
        stock   Adjust component quantity
        remove  Remove a component

        help    Display this menu
        exit    Close PartBuddy
)";
}

/**
 * @brief Collects component information and adds it to the inventory
 * 
 * Prompts the user for component information, quantity, and physical location.
 * Component information will be auto-filled when web-scraping and datasheet parsing is
 * implemented (this is a minimal working version).
 * 
 * @param inventory Inventory instance used to add the component
 * @return true if the component is added successfully, false otherwise.
 */

bool handleAdd(Inventory& inventory)
{
    Component component;

    std::cout << "\n--- Add Component ---\n";

    std::cout << "Manufacturer: ";
    if (!std::getline(std::cin, component.manufacturer))
        return false;

    std::cout << "Part number: ";
    if (!std::getline(std::cin, component.partNumber))
        return false;

    if (component.partNumber.empty())
    {
        std::cout << "Part number cannot be empty.\n";
        return false;
    }

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
 * @brief Routes a command to its corresponding handler
 * 
 * The CLI is responsible for interpreting user commands.
 * Inventory operations are delegated to the inventory class
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
        // Implement list all components, future: specific component type
    }
    else if (command == "search") {
        // Implement component search
    }
    else if (command == "edit") {
        // Implement edit component details
    }
    else if (command == "stock") {
        // Implement changing component stock
    }
    else if (command == "remove") {
        // Implement removing component
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