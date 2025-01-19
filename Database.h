#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

// Alias na typ danych w kolumnach tabeli (int, float, double, bool, string)
using DataValue = std::variant<int, float, double, bool, std::string>;

/**
 * Struktura reprezentująca kolumnę tabeli.
 */
struct Column {
    std::string name; ///< Nazwa kolumny
    std::string type; ///< Typ danych kolumny (INT, STRING, BOOL, FLOAT, DOUBLE)
};

/**
 * Struktura reprezentująca tabelę w bazie danych.
 */
struct Table {
    std::vector<Column> columns; ///< Wektor kolumn tabeli
    std::vector<std::vector<DataValue>> rows; ///< Wektor wierszy tabeli
};

/**
 * Klasa reprezentująca bazę danych.
 * Oferuje funkcje do zarządzania tabelami i ich danymi.
 */
class Database {
    std::unordered_map<std::string, Table> tables; ///< Mapa tabel w bazie danych

    /**
     * Pomocnicza funkcja usuwająca zbędne spacje z początku i końca ciągu znaków.
     * Źródło: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
     * @param str Ciąg znaków do przycięcia
     * @return Ciąg znaków bez zbędnych spacji
     */
    std::string trim(const std::string &str) const;

    /**
     * Konwertuje wartość tekstową na odpowiedni typ danych.
     *
     * @param value Wartość tekstowa do skonwertowania
     * @param type Typ danych (INT, STRING, BOOL, FLOAT, DOUBLE)
     * @return Wartość w odpowiednim typie danych
     * @throws std::invalid_argument Jeśli typ danych jest nieobsługiwany lub wartość niepoprawna
     */
    DataValue parseValue(const std::string &value, const std::string &type) const;

    /**
     * Konwertuje wartość DataValue na std::string
     *
     * @param value Wartość DataValue do skonwertowania
     * @return Ciąg znaków reprezentujący wartość
     */
    std::string dataValueToString(const DataValue &value) const;

public:
    /**
     * Tworzy nową tabelę o podanej nazwie i kolumnach.
     *
     * @param name Nazwa tabeli
     * @param columns Wektor kolumn tabeli
     * @throws std::runtime_error Jeśli tabela o podanej nazwie już istnieje
     */
    void createTable(const std::string &name, const std::vector<Column> &columns);

    /**
     * Usuwa tabelę o podanej nazwie.
     *
     * @param name Nazwa tabeli do usunięcia
     * @throws std::runtime_error Jeśli tabela o podanej nazwie nie istnieje
     */
    void dropTable(const std::string &name);

    /**
     * Dodaje kolumnę do istniejącej tabeli.
     *
     * @param name Nazwa tabeli
     * @param column Struktura kolumny zawierająca jej nazwę i typ
     * @throws std::runtime_error Jeśli tabela o podanej nazwie nie istnieje
     */
    void alterTableAddColumn(const std::string &name, const Column &column);

    /**
     * Wyświetla nazwy wszystkich tabel w bazie danych.
     */
    void showTables() const;

    /**
     * Wstawia nowy wiersz danych do tabeli.
     *
     * @param name Nazwa tabeli
     * @param values Wektor wartości wiersza
     * @throws std::runtime_error Jeśli tabela nie istnieje lub liczba wartości nie pasuje do liczby kolumn
     */
    void insertInto(const std::string &name, const std::vector<std::string> &values);

    /**
     * Wykonuje zapytanie SELECT na istniejącej tabeli.
     *
     * @param name Nazwa tabeli
     * @param columns Wektor kolumn do wyświetlenia (lub `*` dla wszystkich kolumn)
     * @param whereCol Nazwa kolumny w klauzuli WHERE (opcjonalnie)
     * @param whereVal Wartość do porównania w klauzuli WHERE (opcjonalnie)
     * @throws std::runtime_error Jeśli tabela lub kolumna w klauzuli WHERE nie istnieje
     */
    void select(const std::string &name, const std::vector<std::string> &columns,
                const std::string &whereCol = "", const std::string &whereVal = "") const;

    /**
     * Zapisuje wszystkie tabele i ich dane do pliku.
     *
     * @param filename Nazwa pliku
     * @throws std::runtime_error Jeśli plik nie może zostać otwarty
     */
    void saveToFile(const std::string &filename) const;

    /**
     * Ładuje dane tabel z pliku do bazy danych.
     *
     * @param filename Nazwa pliku
     * @throws std::runtime_error Jeśli plik nie może zostać otwarty
     */
    void loadFromFile(const std::string &filename);
};

#endif // DATABASE_H
