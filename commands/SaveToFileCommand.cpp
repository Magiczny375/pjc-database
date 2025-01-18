#include "SaveToFileCommand.h"

#include <sstream>
#include <iostream>

void SaveToFileCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, filename;

    ss >> temp >> filename;

    db.saveToFile(filename);

    std::cout << "Database saved to file: " << filename << "\n";
}
