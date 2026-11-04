#pragma once
#include "Player.h"

// Klasa reprezentująca gracza komputerowego.
// Dziedziczy po klasie bazowej Player i implementuje automatyczny ruch.
class ComputerPlayer : public Player {
public:
    // Konstruktor przyjmujący nazwę gracza (np. "Komputer")
    ComputerPlayer(const std::string& name);

    // Wykonuje ruch automatyczny na planszy
    // Implementacja znajduje się w pliku .cpp
    void makeMove(Board& board) override;
};
