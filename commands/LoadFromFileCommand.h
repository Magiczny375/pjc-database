#ifndef LOADCOMMAND_H
#define LOADCOMMAND_H

#include "ICommand.h"
class LoadFromFileCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // LOADCOMMAND_H
