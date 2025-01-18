#ifndef CREATETABLECOMMAND_H
#define CREATETABLECOMMAND_H

#include "ICommand.h"
#include <sstream>
#include <vector>

class CreateTableCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // CREATETABLECOMMAND_H
