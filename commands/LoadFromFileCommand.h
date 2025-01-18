#ifndef LOADCOMMAND_H
#define LOADCOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę LOAD.
 *
 * Wczytuje dane bazy danych z pliku w formacie tekstowym.
 */
class LoadFromFileCommand final : public ICommand {
public:
    /**
     * Wczytuje dane bazy danych z pliku.
     *
     * @param db Obiekt bazy danych, do którego dane mają zostać załadowane
     * @param query Zapytanie użytkownika w formacie LOAD FROM filename
     * @throws std::runtime_error Jeśli pliku nie można otworzyć
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // LOADCOMMAND_H
