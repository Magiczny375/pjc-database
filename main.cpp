#include <iostream>
#include <string>
#include <sstream>
#include "Database.h"
#include "CommandExecutor.h"
#include "commands/CreateTableCommand.h"
#include "commands/DropTableCommand.h"
#include "commands/AlterTableCommand.h"
#include "commands/ShowTablesCommand.h"
#include "commands/InsertCommand.h"
#include "commands/SaveToFileCommand.h"
#include "commands/LoadFromFileCommand.h"
#include "commands/SelectCommand.h"

// *** Struktura danych i opis działania aplikacji ***
//
// 1. Dane przechowywane są w pamięci operacyjnej w formie mapy tabel. Każda tabela ma:
//    - nazwę
//    - kolumny (nazwa i typ danych)
//    - wiersze (wartości w formie wektorów odpowiadających kolumnom)
//
// 2. Obsługiwane operacje (prosty język zapytań, inspirowany SQL):
//    - DDL:
//      * CREATE TABLE table_name (col_name col_type, ...)
//      * DROP TABLE table_name
//      * ALTER TABLE table_name ADD COLUMN col_name col_type
//      * SHOW TABLES
//    - DML:
//      * INSERT INTO table_name VALUES (value1, value2, ...)
//    - DQL:
//      * SELECT col1, col2 FROM table_name WHERE col_name = value
//      * SELECT * FROM table_name
//    - Zapis/odczyt stanu:
//      * SAVE TO file_name
//      * LOAD FROM file_name
//
// 3. Obsługiwane typy danych: INT, STRING, BOOL, FLOAT, DOUBLE
// 4. Weryfikacja typów danych odbywa się podczas wstawiania wierszy.
// 5. Wszystkie dane są wczytywane/zapisywane z/do pliku na żądanie.
// 6. Dane z aplikacji są zapisywanie w momencie wyjścia z CLI przez użytkownika - wykonanie komendy EXIT/exit.
// 6.1. Wczytywanie w dalszym ciągu nie jest wymagane/automatyczne. Użytkownik sam może wgrać dane komendą LOAD.
// 6.2. Folder i ścieżka, w którym zapisywane są pliki z danymi to cmake-build-debug/[nazwa pliku]
// ----
// 7. W celu trzymania się chociaż trochę zasad clean code + SOLID użyłem wzorca komendy [Command pattern]
// ...i rozdzieliłem wszystkie operacje [SELECT, DROP, SHOW, etc.] na odpowiadające im komendy. Struktura:
//  - commands/*: folder zawierający header ICommand służący jako klasa abstrakcyjna dla wszystkich komend.
//  - Database.h - header file / interfejs dla klasy managera bazy danych.
//  - CommandExecutor.h - header file / rejestr i egzekutor komend ICommand.h

int main() {
    Database db;
    CommandExecutor executor;

    // Rejestracja komend. Można tutaj pewnie dodać enum class dla typu, ale brak funduszy w firmie :D.
    // Rejestracja klas odbywa się z użyciem inteligentnego wskaźnika,
    // ...dzięki temu trochę mniej martwię się o memory leak.
    executor.registerCommand("CREATE", std::make_unique<CreateTableCommand>());
    executor.registerCommand("DROP", std::make_unique<DropTableCommand>());
    executor.registerCommand("ALTER", std::make_unique<AlterTableCommand>());
    executor.registerCommand("SHOW", std::make_unique<ShowTablesCommand>());
    executor.registerCommand("SELECT", std::make_unique<SelectCommand>());
    executor.registerCommand("INSERT", std::make_unique<InsertCommand>());
    executor.registerCommand("SAVE", std::make_unique<SaveToFileCommand>());
    executor.registerCommand("LOAD", std::make_unique<LoadFromFileCommand>());

    std::string query;
    std::cout << "PJCDatabase >> Author: Krystian Dębek | Version: 1.0\n";
    std::cout << "PJCDatabase >> Welcome to the PJC database system. Type your commands below.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, query);

        if (query == "EXIT" || query == "exit") {
            // Automatyczny zapis danych z aplikacji przy wyjściu.
            auto const command = "SAVE";
            auto const params = "TO database.txt";
            executor.executeCommand(command, db, params);
            break;
        }

        try {
            // Źródło: https://stackoverflow.com/a/53491679
            std::istringstream ss(query);
            std::string commandName, params;

            ss >> commandName;
            std::getline(ss, params);

            executor.executeCommand(commandName, db, params);
        } catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
