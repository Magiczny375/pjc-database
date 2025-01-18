#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include "ICommand.h"
#include <sstream>

/**
 * Klasa reprezentująca komendę SAVE.
 *
 * Zapisuje dane bazy danych do pliku w formacie tekstowym.
 */
class SaveToFileCommand final : public ICommand {
public:
    /**
     * Zapisuje dane bazy danych do pliku.
     *
     * @param db Obiekt bazy danych, który ma zostać zapisany
     * @param query Zapytanie użytkownika w formacie SAVE TO filename
     * @throws std::runtime_error Jeśli pliku nie można otworzyć
     */
    void execute(Database &db, const std::string &query) override;
};

#endif // SAVECOMMAND_H
