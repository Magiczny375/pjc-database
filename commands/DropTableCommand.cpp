#include "DropTableCommand.h"

#include <iostream>
#include <sstream>

void DropTableCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, tableName;

    ss >> temp >> tableName;

    db.dropTable(tableName);

    std::cout << "Table dropped.\n";
}
