#ifndef ALTERTABLECOMMAND_H
#define ALTERTABLECOMMAND_H

#include "ICommand.h"

class AlterTableCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // ALTERTABLECOMMAND_H
