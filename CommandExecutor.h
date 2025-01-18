#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <unordered_map>
#include <memory>
#include <string>
#include "commands/ICommand.h"

class CommandExecutor {
    std::unordered_map<std::string, std::unique_ptr<ICommand> > commands;

public:
    void registerCommand(const std::string &name, std::unique_ptr<ICommand> command) {
        commands[name] = std::move(command);
    }

    void executeCommand(const std::string &name, Database &db, const std::string &query) {
        auto it = commands.find(name);
        if (it == commands.end()) {
            throw std::runtime_error("Command not found: " + name);
        }
        it->second->execute(db, query);
    }
};

#endif // COMMANDEXECUTOR_H
