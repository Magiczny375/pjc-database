#include "SelectCommand.h"
#include <iostream>

void SelectCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, columnsDef, tableName, whereClause, whereCol, whereVal;

    // Parse the SELECT command
    ss >> columnsDef;
    ss >> temp >> tableName;

    // Columns to select (e.g., "col1,col2" or "*")
    std::vector<std::string> columns;
    if (columnsDef == "*") {
        columns.push_back("*");       // Select all columns
    } else {
        std::istringstream colStream(columnsDef);
        std::string column;
        while (std::getline(colStream, column, ',')) {
            columns.push_back(column); // Add each column to the list
        }
    }

    // Optional WHERE clause
    if (ss >> temp && temp == "WHERE") {
        ss >> whereCol >> temp >> whereVal; // e.g., col_name = value
    }

    db.select(tableName, columns, whereCol, whereVal);
}
