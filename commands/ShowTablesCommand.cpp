#include "ShowTablesCommand.h"

void ShowTablesCommand::execute(Database &db, const std::string &query) {
    db.showTables();
}
