#ifndef DROPTABLECOMMAND_H
#define DROPTABLECOMMAND_H

#include "ICommand.h"

class DropTableCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // DROPTABLECOMMAND_H
