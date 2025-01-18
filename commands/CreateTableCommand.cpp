#include "CreateTableCommand.h"

void CreateTableCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, tableName;
    ss >> temp >> tableName; // Skipping "CREATE TABLE"
    std::string columnsDef;
    std::getline(ss, columnsDef, '(');
    std::getline(ss, columnsDef, ')');

    std::istringstream colsStream(columnsDef);
    std::string colDef;
    std::vector<Column> columns;
    while (std::getline(colsStream, colDef, ',')) {
        std::istringstream colStream(colDef);
        std::string colName, colType;
        colStream >> colName >> colType;
        columns.push_back({colName, colType});
    }
    db.createTable(tableName, columns);
    std::cout << "Table created.\n";
}
