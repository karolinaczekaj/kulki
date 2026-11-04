#pragma once
#include "Board.h"
#include "Player.h"
#include <memory>

// klasa ogarniajaca cala gre
class Game {
private:
    Board board; // plansza do gry
    std::unique_ptr<Player> player1; // pierwszy gracz
    std::unique_ptr<Player> player2; // drugi gracz (albo komputer albo null)

    bool loadSavedGame(); // wczytuje zapis z pliku jak jest
    std::string player1Name, player2Name; // nicki graczy
    bool isPlayer1Turn; // flaga kto ma ruch

public:
    Game(); // konstruktor, tworzy plansze i ustawia start
    void run(); // glowna petla gry
    void saveGameState(); // zapisuje stan gry do pliku
};
