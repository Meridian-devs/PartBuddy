#include <iostream>
#include <string>
#include <unordered_map>

void printWelcome()
{
    std::cout << R"(
    888888ba                      dP    888888ba                 dP       dP          
    88    `8b                     88    88    `8b                88       88          
    a88aaaa8P' .d8888b. 88d888b. d8888P a88aaaa8P'dP    dP .d888b88 .d888b88 dP    dP 
    88        88'  `88 88'  `88   88    88   `8b. 88    88 88'  `88 88'  `88 88    88 
    88        88.  .88 88         88    88    .88 88.  .88 88.  .88 88.  .88 88.  .88 
    dP        `88888P8 dP         dP    88888888P `88888P' `88888P8 `88888P8 `8888P88 
                                                                               .88 
                                                                           d8888P
                                                                           
    Your electronics inventory manager. Type 'help' to see available commands.
)";
}

void printHelp()
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

void handleCommand(const std::string& command)
{
    if (command == "help") {
        printHelp();                                        // Prints list of commands
    }
    else if (command == "add") {
        // Implement add component
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

int main()
{
    printWelcome();                                         // Print welcome graphic and message
    std::string command;                                    // Init the command variable

    while (true)
    {
        std::cout << "PartBuddy > ";
        if (!std::getline(std::cin, command)) {

        }
        if (command == "exit" || command == "quit") {
            break;                                          // Handle exit/quit command
        }

        handleCommand(command);
    }
    std::cout << "\nGoodbye!\n";                            // If exit requested, print goodbye message
    return 0;
}

// cmake -S . -B build -G Ninja
// To build project, utilize CMake command: cmake --build build