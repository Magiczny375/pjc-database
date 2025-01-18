#ifndef DROPTABLECOMMAND_H
#define DROPTABLECOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę DROP TABLE.
 *
 * Usuwa tabelę z bazy danych na podstawie jej nazwy.
 */
class DropTableCommand final : public ICommand {
public:
    /**
     * Usuwa tabelę z bazy danych.
     *
     * @param db Obiekt bazy danych, z którego zostanie usunięta tabela
     * @param query Zapytanie użytkownika w formacie DROP TABLE table_name
     * @throws std::runtime_error Jeśli tabela o podanej nazwie nie istnieje
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // DROPTABLECOMMAND_H
