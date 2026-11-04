#pragma once
#include "Board.h"
#include <string>

// bazowa klasa dla graczy (czlowiek/komputer)
// glownie trzyma nick i wynik
class Player {
protected:
    std::string name; // nick gracza
    int score = 0;    // punkty tego gracza

public:
    Player(const std::string& n);              // konstruktor z nickiem
    virtual void makeMove(Board& board) = 0;   // kazdy gracz musi miec swoj ruch
    virtual ~Player() = default;               // zeby sie ladnie niszczylo
    void addScore(int s) { score += s; }       // dodaje punkty
    int getScore() const { return score; }     // zwraca ile punktow ma gracz
    std::string getName() const { return name; } // zwraca nick
};
