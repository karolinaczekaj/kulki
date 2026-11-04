#pragma once
#include "Player.h"

// klasa dla gracza, dziedziczy po Player
class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name); // konstruktor, przekazuje nick
    void makeMove(Board& board) override; // to co robi gracz w swojej turze
};

