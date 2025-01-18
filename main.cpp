#include <iostream>
#include <string>
#include <sstream>
#include "Database.h"
#include "CommandExecutor.h"
#include "commands/CreateTableCommand.h"
#include "commands/DropTableCommand.h"
#include "commands/AlterTableCommand.h"
#include "commands/ShowTablesCommand.h"
#include "commands/InsertCommand.h"
#include "commands/SaveToFileCommand.h"
#include "commands/LoadFromFileCommand.h"
#include "commands/SelectCommand.h"

int main() {
    Database db;
    CommandExecutor executor;

    // Rejestracja komend.
    executor.registerCommand("CREATE", std::make_unique<CreateTableCommand>());
    executor.registerCommand("DROP", std::make_unique<DropTableCommand>());
    executor.registerCommand("ALTER", std::make_unique<AlterTableCommand>());
    executor.registerCommand("SHOW", std::make_unique<ShowTablesCommand>());
    executor.registerCommand("SELECT", std::make_unique<SelectCommand>());
    executor.registerCommand("INSERT", std::make_unique<InsertCommand>());
    executor.registerCommand("SAVE", std::make_unique<SaveToFileCommand>());
    executor.registerCommand("LOAD", std::make_unique<LoadFromFileCommand>());

    std::string query;
    std::cout << "Database | version 1.0\nWelcome to the database system. Type your queries below.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, query);
        if (query == "EXIT") {
            break;
        }

        try {
            std::istringstream ss(query);
            std::string commandName, params;

            ss >> commandName;
            std::getline(ss, params);

            executor.executeCommand(commandName, db, params);
        } catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
