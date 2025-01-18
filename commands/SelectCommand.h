#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include "ICommand.h"
#include <string>

class SelectCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // SELECTCOMMAND_H
