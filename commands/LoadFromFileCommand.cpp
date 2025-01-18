#include "LoadFromFileCommand.h"
#include <iostream>

void LoadFromFileCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, filename;
    ss >> temp >> filename; // Skip "LOAD FROM"
    db.loadFromFile(filename);
    std::cout << "Database loaded from file: " << filename << "\n";
}
