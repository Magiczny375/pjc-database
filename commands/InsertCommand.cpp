#include "InsertCommand.h"

#include <iostream>
#include <sstream>

void InsertCommand::execute(Database &db, const std::string &query) {
    std::istringstream ss(query);
    std::string temp, tableName;

    ss >> temp >> tableName;

    std::string valuesDef;
    std::getline(ss, valuesDef, '(');
    std::getline(ss, valuesDef, ')');

    std::istringstream valsStream(valuesDef);
    std::string value;
    std::vector<std::string> values;

    while (std::getline(valsStream, value, ',')) {
        values.push_back(value);
    }

    db.insertInto(tableName, values);

    std::cout << "Row inserted.\n";
}
