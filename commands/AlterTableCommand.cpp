#include "AlterTableCommand.h"

void AlterTableCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, tableName;
    ss >> temp >> tableName; // Skipping "ALTER TABLE"
    std::string alterCmd, colName, colType;
    ss >> alterCmd >> temp >> colName >> colType; // ADD COLUMN column_name type
    db.alterTableAddColumn(tableName, {colName, colType});
    std::cout << "Table altered.\n";
}
