#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę INSERT INTO.
 *
 * Wstawia nowy wiersz danych do istniejącej tabeli w bazie danych.
 */
class InsertCommand final : public ICommand {
public:
    /**
     * Wstawia nowy wiersz do tabeli.
     *
     * @param db Obiekt bazy danych, do którego tabela należy
     * @param query Zapytanie użytkownika w formacie INSERT INTO table_name VALUES (value1, value2, ...)
     * @throws std::runtime_error Jeśli tabela nie istnieje lub liczba wartości nie pasuje do liczby kolumn
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // INSERTCOMMAND_H
