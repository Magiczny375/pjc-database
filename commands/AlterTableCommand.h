#ifndef ALTERTABLECOMMAND_H
#define ALTERTABLECOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę ALTER TABLE.
 *
 * Dodaje nową kolumnę do istniejącej tabeli w bazie danych.
 */
class AlterTableCommand final : public ICommand {
public:
    /**
     * Dodaje kolumnę do istniejącej tabeli w bazie danych.
     *
     * @param db Obiekt bazy danych, w którym modyfikowana jest tabela
     * @param query Zapytanie użytkownika w formacie ALTER TABLE table_name ADD COLUMN col_name col_type
     * @throws std::runtime_error Jeśli tabela o podanej nazwie nie istnieje
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // ALTERTABLECOMMAND_H
