#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <fmt/core.h>

// DataValue type to represent any column data
using DataValue = std::variant<int, float, double, bool, std::string>;

struct Column {
    std::string name;
    std::string type;
};

struct Table {
    std::vector<Column> columns;
    std::vector<std::vector<DataValue>> rows;
};

class Database {
private:
    std::unordered_map<std::string, Table> tables;

    // Helper to trim strings
    std::string trim(const std::string &str) const;

    // Parse a value from string to DataValue based on type
    DataValue parseValue(const std::string &value, const std::string &type) const;

    // Convert DataValue to string for display
    std::string dataValueToString(const DataValue &value) const;

public:
    void createTable(const std::string &name, const std::vector<Column> &columns);
    void dropTable(const std::string &name);
    void alterTableAddColumn(const std::string &name, const Column &column);
    void showTables() const;
    void insertInto(const std::string &name, const std::vector<std::string> &values);
    void select(const std::string &name, const std::vector<std::string> &columns, const std::string &whereCol = "",
                const std::string &whereVal = "") const;
    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};

#endif // DATABASE_H
