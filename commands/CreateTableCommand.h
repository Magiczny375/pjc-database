#ifndef CREATETABLECOMMAND_H
#define CREATETABLECOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę CREATE TABLE.
 *
 * Tworzy nową tabelę w obiekcie bazy danych.
 * Analizuje zapytanie w celu wydobycia nazwy tabeli i jej kolumn.
 */
class CreateTableCommand final : public ICommand {
public:
    /**
     * Tworzy nową tabelę w bazie danych.
     *
     * @param db Obiekt bazy danych, w którym zostanie utworzona tabela
     * @param query Zapytanie użytkownika w formacie CREATE TABLE table_name (col_name col_type, ...)
     * @throws std::runtime_error Jeśli tabela o podanej nazwie już istnieje
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // CREATETABLECOMMAND_H
