#include "SelectCommand.h"

#include <sstream>
#include <iostream>

void SelectCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, columnsDef, tableName, whereClause, whereCol, whereVal;

    ss >> columnsDef;
    ss >> temp >> tableName;

    std::vector<std::string> columns;
    if (columnsDef == "*") {
        columns.push_back("*");
    } else {
        std::istringstream colStream(columnsDef);
        std::string column;

        while (std::getline(colStream, column, ',')) {
            columns.push_back(column);
        }
    }

    if (ss >> temp && temp == "WHERE") {
        ss >> whereCol >> temp >> whereVal;
    }

    db.select(tableName, columns, whereCol, whereVal);
}
