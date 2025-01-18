#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include "commands/ICommand.h"

/**
 * Klasa odpowiedzialna za rejestrację i wykonanie komend.
 *
 * CommandExecutor pełni funkcję menedżera komend:
 * - Umożliwia rejestrację komend za pomocą unikalnych nazw (np. "CREATE", "INSERT").
 * - Znajduje odpowiednią komendę na podstawie nazwy.
 * - Wykonuje logikę komend, wywołując metodę `execute` odpowiedniej klasy komendy.
 *
 * Komendy są przechowywane w mapie:
 * - Klucz: `std::string` (nazwa operacji, np. "CREATE").
 * - Wartość: `std::unique_ptr<ICommand>` (wskaźnik do obiektu implementującego interfejs `ICommand`).
 */
class CommandExecutor {
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commands; ///< Mapa przechowująca komendy

public:
    /**
     * Rejestruje nową komendę w mapie.
     *
     * @param name Unikalna nazwa komendy (np. "CREATE", "DROP").
     * @param command Inteligentny wskaźnik do obiektu implementującego `ICommand`.
     */
    void registerCommand(const std::string &name, std::unique_ptr<ICommand> command) {
        commands[name] = std::move(command);
    }

    /**
     * Wykonuje komendę na podstawie jej nazwy.
     *
     * @param name Nazwa komendy do wykonania
     * @param db Obiekt bazy danych, na którym wykonywana jest operacja
     * @param query Zapytanie użytkownika (pełne zapytanie tekstowe)
     * @throws std::runtime_error Jeśli komenda o podanej nazwie nie została zarejestrowana
     */
    void executeCommand(const std::string &name, Database &db, const std::string &query) {
        // Znajdź komendę w mapie na podstawie jej nazwy
        auto const command = commands.find(name);

        // Jeśli komenda nie istnieje, zgłoś wyjątek
        if (command == commands.end()) {
            throw std::runtime_error("Command not found for provided name: " + name);
        }

        // Wywołaj logikę komendy
        // Użycie command->second pozwala uzyskać dostęp do obiektu komendy,
        // ponieważ unordered_map przechowuje dane w postaci pary klucz-wartość.
        command->second->execute(db, query);
    }
};

#endif // COMMANDEXECUTOR_H
