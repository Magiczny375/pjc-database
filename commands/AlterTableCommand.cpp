#include "AlterTableCommand.h"

#include <iostream>
#include <sstream>

void AlterTableCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, tableName;

    ss >> temp >> tableName;
    std::string alterCmd, colName, colType;

    ss >> alterCmd >> temp >> colName >> colType;

    db.alterTableAddColumn(tableName, {colName, colType});

    std::cout << "Table altered.\n";
}
