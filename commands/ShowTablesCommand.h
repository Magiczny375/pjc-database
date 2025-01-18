#ifndef SHOWTABLESCOMMAND_H
#define SHOWTABLESCOMMAND_H

#include "ICommand.h"

class ShowTablesCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // SHOWTABLESCOMMAND_H
