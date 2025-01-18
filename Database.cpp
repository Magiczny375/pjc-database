#include "Database.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <fmt/core.h>
#include <sstream>

// Trim a string to remove leading/trailing spaces
std::string Database::trim(const std::string &str) const {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return start == std::string::npos || end == std::string::npos ? "" : str.substr(start, end - start + 1);
}

// Parse a string value into the appropriate DataValue type
DataValue Database::parseValue(const std::string &value, const std::string &type) const {
    auto trimmedValue = trim(value);
    trimmedValue.erase(std::remove(trimmedValue.begin(), trimmedValue.end(), '"'), trimmedValue.end());

    if (type == "INT") {
        return std::stoi(trimmedValue);
    }
    if (type == "STRING") {
        return trimmedValue;
    }
    if (type == "BOOL") {
        if (trimmedValue == "true" || trimmedValue == "1") return true;
        if (trimmedValue == "false" || trimmedValue == "0") return false;
        throw std::invalid_argument("Invalid BOOL value");
    }
    if (type == "FLOAT") {
        return std::stof(trimmedValue);
    }
    if (type == "DOUBLE") {
        return std::stod(trimmedValue);
    }
    throw std::invalid_argument("Unknown type: " + type);
}

// Convert a DataValue to string for display
std::string Database::dataValueToString(const DataValue &value) const {
    return std::visit([](const auto &val) { return fmt::format("{}", val); }, value);
}

// Create a new table with the given name and columns
void Database::createTable(const std::string &name, const std::vector<Column> &columns) {
    if (tables.find(name) != tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' already exists", name));
    }
    tables[name] = {columns, {}};
}

// Drop a table by its name
void Database::dropTable(const std::string &name) {
    if (tables.erase(name) == 0) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
}

// Add a column to an existing table
void Database::alterTableAddColumn(const std::string &name, const Column &column) {
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    it->second.columns.push_back(column);
    for (auto &row: it->second.rows) {
        row.push_back(DataValue{}); // Add an empty value for existing rows
    }
}

// Display the names of all tables in the database
void Database::showTables() const {
    if (tables.empty()) {
        std::cout << "No tables in database.\n";
        return;
    }
    std::cout << "List of tables:\n";
    for (const auto &[name, _]: tables) {
        std::cout << name << "\n";
    }
}

// Insert a row of values into a table
void Database::insertInto(const std::string &name, const std::vector<std::string> &values) {
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    const auto &columns = it->second.columns;
    if (values.size() != columns.size()) {
        throw std::runtime_error("Value count does not match column count");
    }
    std::vector<DataValue> row;
    for (size_t i = 0; i < values.size(); ++i) {
        row.push_back(parseValue(values[i], columns[i].type));
    }
    it->second.rows.push_back(row);
}

// Select rows and columns from a table, with an optional WHERE condition
void Database::select(const std::string &name, const std::vector<std::string> &columns, const std::string &whereCol,
                      const std::string &whereVal) const {
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    const auto &table = it->second;

    // Get column indexes for the SELECT clause
    std::vector<size_t> colIndexes;
    if (columns.size() == 1 && columns[0] == "*") {
        for (size_t i = 0; i < table.columns.size(); ++i) {
            colIndexes.push_back(i);
        }
    } else {
        for (const auto &col: columns) {
            auto it = std::find_if(table.columns.begin(), table.columns.end(),
                                   [&col](const Column &c) { return c.name == col; });
            if (it == table.columns.end()) {
                throw std::runtime_error(fmt::format("Column '{}' does not exist", col));
            }
            colIndexes.push_back(std::distance(table.columns.begin(), it));
        }
    }

    // Print column headers
    fmt::println("Table: {}", name);
    for (size_t idx: colIndexes) {
        fmt::print("{}\t", table.columns[idx].name);
    }
    fmt::print("\n");
    for (size_t idx: colIndexes) {
        fmt::print("--------");
    }
    fmt::print("\n");

    // Print rows that satisfy the WHERE condition
    for (const auto &row: table.rows) {
        bool whereCondition = true;
        if (!whereCol.empty()) {
            auto it = std::find_if(table.columns.begin(), table.columns.end(),
                                   [&whereCol](const Column &c) { return c.name == whereCol; });
            if (it == table.columns.end()) {
                throw std::runtime_error(fmt::format("Column '{}' does not exist in WHERE clause", whereCol));
            }
            size_t whereIndex = std::distance(table.columns.begin(), it);
            whereCondition = (row[whereIndex] == parseValue(whereVal, it->type));
        }

        if (whereCondition) {
            for (size_t idx: colIndexes) {
                fmt::print("{}\t", dataValueToString(row[idx]));
            }
            fmt::print("\n");
        }
    }

    for (size_t idx: colIndexes) {
        fmt::print("--------");
    }
    fmt::print("\n");
}

// Save the database to a file
void Database::saveToFile(const std::string &filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error(fmt::format("Could not open file '{}'", filename));
    }
    for (const auto &[name, table]: tables) {
        out << "TABLE " << name << "\n";
        for (const auto &col: table.columns) {
            out << "COLUMN " << col.name << " " << col.type << "\n";
        }
        for (const auto &row: table.rows) {
            out << "ROW ";
            for (const auto &val: row) {
                out << dataValueToString(val) << " ";
            }
            out << "\n";
        }
    }
}

// Load the database from a file
void Database::loadFromFile(const std::string &filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error(fmt::format("Could not open file '{}'", filename));
    }
    tables.clear();
    std::string line;
    Table *currentTable = nullptr;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;
        if (keyword == "TABLE") {
            std::string name;
            ss >> name;
            tables[name] = {};
            currentTable = &tables[name];
        } else if (keyword == "COLUMN") {
            std::string name, type;
            ss >> name >> type;
            currentTable->columns.push_back({name, type});
        } else if (keyword == "ROW") {
            std::vector<DataValue> row;
            for (const auto &col: currentTable->columns) {
                std::string value;
                ss >> value;
                row.push_back(parseValue(value, col.type));
            }
            currentTable->rows.push_back(row);
        }
    }
}
