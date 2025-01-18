#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include "../Database.h"

/**
 * Interfejs bazowy dla wszystkich komend.
 *
 * Klasy dziedziczące po `ICommand` muszą implementować metodę `execute`.
 * Każda komenda powinna wykonywać określoną operację na obiekcie `Database`.
 */
class ICommand {
public:
    /**
     * Wykonuje logikę komendy.
     *
     * @param db Obiekt bazy danych, na którym wykonywana jest operacja
     * @param query Pełne zapytanie użytkownika
     */
    virtual void execute(Database &db, const std::string &query) = 0;

    /// Wirtualny destruktor dla poprawnego zarządzania dziedziczeniem
    virtual ~ICommand() = default;
};

#endif // ICOMMAND_H
