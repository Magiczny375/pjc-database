#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include "../Database.h"

// Command interface for all database commands
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Database &db, const std::string &query) = 0;
};

#endif // ICOMMAND_H
