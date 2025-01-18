#include "Database.h"

#include <algorithm>  // std::find_if, std::remove
#include <fstream>    // std::ofstream, std::ifstream
#include <iostream>   // std::cout
#include <stdexcept>  // std::runtime_error, std::invalid_argument
#include <fmt/core.h> // Biblioteka FMT do formatowania napisów
#include <sstream>    // std::istringstream

std::string Database::trim(const std::string &str) const {
    // Znajdź pierwszy i ostatni znak, który nie jest spacją
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    // Zwróć przycięty ciąg znaków
    return start == std::string::npos || end == std::string::npos ? "" : str.substr(start, end - start + 1);
}

DataValue Database::parseValue(const std::string &value, const std::string &type) const {
    // Usuń nadmiarowe spacje i cudzysłowy
    auto trimmedValue = trim(value);
    trimmedValue.erase(std::remove(trimmedValue.begin(), trimmedValue.end(), '"'), trimmedValue.end());

    // Konwersja na odpowiedni typ danych
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

    // Jeśli typ danych nie jest obsługiwany, zgłoś wyjątek
    throw std::invalid_argument("Unknown type: " + type);
}

std::string Database::dataValueToString(const DataValue &value) const {
    // Użyj std::visit, aby skonwertować wartość std::variant na string
    return std::visit([](const auto &val) { return fmt::format("{}", val); }, value);
}

void Database::createTable(const std::string &name, const std::vector<Column> &columns) {
    // Sprawdź, czy tabela już istnieje
    if (tables.find(name) != tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' already exists", name));
    }
    // Dodaj nową tabelę do mapy
    tables[name] = {columns, {}};
}

void Database::dropTable(const std::string &name) {
    // Usuń tabelę z mapy, zgłoś wyjątek, jeśli tabela nie istnieje
    if (tables.erase(name) == 0) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
}

void Database::alterTableAddColumn(const std::string &name, const Column &column) {
    // Znajdź tabelę w mapie
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    // Dodaj kolumnę do tabeli
    it->second.columns.push_back(column);
    for (auto &row : it->second.rows) {
        row.push_back(DataValue{}); // Dodaj pustą wartość dla istniejących wierszy
    }
}

void Database::showTables() const {
    // Jeśli baza danych jest pusta, wyświetl komunikat
    if (tables.empty()) {
        std::cout << "No tables in database.\n";
        return;
    }
    // Wyświetl nazwy wszystkich tabel
    std::cout << "List of tables:\n";
    for (const auto &[name, _] : tables) {
        std::cout << name << "\n";
    }
}

void Database::insertInto(const std::string &name, const std::vector<std::string> &values) {
    // Znajdź tabelę i sprawdź, czy istnieje
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    const auto &columns = it->second.columns;

    // Sprawdź, czy liczba wartości pasuje do liczby kolumn
    if (values.size() != columns.size()) {
        throw std::runtime_error("Value count does not match column count");
    }

    // Konwertuj wartości na odpowiednie typy i dodaj wiersz
    std::vector<DataValue> row;
    for (size_t i = 0; i < values.size(); ++i) {
        row.push_back(parseValue(values[i], columns[i].type));
    }
    it->second.rows.push_back(row);
}

void Database::select(const std::string &name, const std::vector<std::string> &columns,
                      const std::string &whereCol, const std::string &whereVal) const {
    // Znajdź tabelę i sprawdź, czy istnieje
    auto it = tables.find(name);
    if (it == tables.end()) {
        throw std::runtime_error(fmt::format("Table '{}' does not exist", name));
    }
    const auto &table = it->second;

    // Wybierz indeksy kolumn do wyświetlenia
    std::vector<size_t> colIndexes;
    if (columns.size() == 1 && columns[0] == "*") {
        for (size_t i = 0; i < table.columns.size(); ++i) {
            colIndexes.push_back(i);
        }
    } else {
        for (const auto &col : columns) {
            auto colIt = std::find_if(table.columns.begin(), table.columns.end(),
                                      [&col](const Column &c) { return c.name == col; });
            if (colIt == table.columns.end()) {
                throw std::runtime_error(fmt::format("Column '{}' does not exist", col));
            }
            colIndexes.push_back(std::distance(table.columns.begin(), colIt));
        }
    }

    // Wyświetl nagłówki kolumn
    fmt::println("Table: {}", name);
    for (size_t idx : colIndexes) {
        fmt::print("{}\t", table.columns[idx].name);
    }
    fmt::print("\n");
    for (size_t idx : colIndexes) {
        fmt::print("--------");
    }
    fmt::print("\n");

    // Filtrowanie wierszy na podstawie warunku WHERE (jeśli istnieje)
    for (const auto &row : table.rows) {
        bool whereCondition = true;
        if (!whereCol.empty()) {
            auto colIt = std::find_if(table.columns.begin(), table.columns.end(),
                                      [&whereCol](const Column &c) { return c.name == whereCol; });
            if (colIt == table.columns.end()) {
                throw std::runtime_error(fmt::format("Column '{}' does not exist in WHERE clause", whereCol));
            }
            size_t whereIndex = std::distance(table.columns.begin(), colIt);
            whereCondition = (row[whereIndex] == parseValue(whereVal, colIt->type));
        }

        // Wyświetl wiersz, jeśli spełnia warunek WHERE
        if (whereCondition) {
            for (size_t idx : colIndexes) {
                fmt::print("{}\t", dataValueToString(row[idx]));
            }
            fmt::print("\n");
        }
    }

    // Wyświetl separator końcowy
    for (size_t idx : colIndexes) {
        fmt::print("--------");
    }
    fmt::print("\n");
}

void Database::saveToFile(const std::string &filename) const {
    // Otwórz plik do zapisu
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error(fmt::format("Could not open file '{}'", filename));
    }

    // Zapisz dane każdej tabeli do pliku
    for (const auto &[name, table] : tables) {
        out << "TABLE " << name << "\n";
        for (const auto &col : table.columns) {
            out << "COLUMN " << col.name << " " << col.type << "\n";
        }
        for (const auto &row : table.rows) {
            out << "ROW ";
            for (const auto &val : row) {
                out << dataValueToString(val) << " ";
            }
            out << "\n";
        }
    }
}

void Database::loadFromFile(const std::string &filename) {
    // Otwórz plik do odczytu
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error(fmt::format("Could not open file '{}'", filename));
    }

    // Wyczyść istniejące dane w bazie
    tables.clear();
    std::string line;
    Table *currentTable = nullptr;

    // Wczytaj linie z pliku i parsuj dane tabel
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;
        if (keyword == "TABLE") {
            // Utwórz nową tabelę
            std::string name;
            ss >> name;
            tables[name] = {};
            currentTable = &tables[name];
        } else if (keyword == "COLUMN") {
            // Dodaj kolumnę do aktualnej tabeli
            std::string name, type;
            ss >> name >> type;
            currentTable->columns.push_back({name, type});
        } else if (keyword == "ROW") {
            // Dodaj wiersz do aktualnej tabeli
            std::vector<DataValue> row;
            for (const auto &col : currentTable->columns) {
                std::string value;
                ss >> value;
                row.push_back(parseValue(value, col.type));
            }
            currentTable->rows.push_back(row);
        }
    }
}
