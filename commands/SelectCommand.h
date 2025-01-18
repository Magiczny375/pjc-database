#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę SELECT.
 *
 * Pobiera dane z tabeli na podstawie wybranych kolumn i opcjonalnego warunku WHERE.
 */
class SelectCommand final : public ICommand {
public:
    /**
     * Pobiera dane z tabeli.
     *
     * @param db Obiekt bazy danych, z którego pobierane są dane
     * @param query Zapytanie użytkownika w formacie SELECT col1, col2 FROM table_name WHERE col_name = value
     * @throws std::runtime_error Jeśli tabela lub kolumna w klauzuli WHERE nie istnieje
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // SELECTCOMMAND_H
