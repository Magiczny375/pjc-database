#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include "ICommand.h"

class InsertCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // INSERTCOMMAND_H
