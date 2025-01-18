#ifndef SHOWTABLESCOMMAND_H
#define SHOWTABLESCOMMAND_H

#include "ICommand.h"

/**
 * Klasa reprezentująca komendę SHOW TABLES.
 *
 * Wyświetla nazwy wszystkich tabel w bazie danych.
 */
class ShowTablesCommand final : public ICommand {
public:
    /**
     * Wyświetla nazwy wszystkich tabel w bazie danych.
     *
     * @param db Obiekt bazy danych, w którym wyświetlane są tabele
     * @param query Ignorowane (SHOW TABLES nie wymaga dodatkowych argumentów)
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // SHOWTABLESCOMMAND_H
