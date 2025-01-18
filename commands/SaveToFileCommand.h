#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include "ICommand.h"

class SaveToFileCommand : public ICommand {
public:
    void execute(Database &db, const std::string &query) override;
};

#endif // SAVECOMMAND_H
