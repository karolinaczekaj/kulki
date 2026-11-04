#include "ComputerPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

ComputerPlayer::ComputerPlayer(const std::string& n) : Player(n) {
    // inicjalizacja generatora liczb losowych
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void ComputerPlayer::makeMove(Board& board) {
    std::cout << name << " wykonuje ruch automatyczny...\n";

    struct Move {
        int fromX, fromY, toX, toY;
    };

    std::vector<Move> possibleMoves;

    // znajdz wszystkie mozliwe ruchy z kazdego pola na inne
    for (int y1 = 0; y1 < 9; ++y1) {
        for (int x1 = 0; x1 < 9; ++x1) {
            if (!board.isFree(x1, y1)) {
                for (int y2 = 0; y2 < 9; ++y2) {
                    for (int x2 = 0; x2 < 9; ++x2) {
                        if (board.isFree(x2, y2) && board.pathExists(x1, y1, x2, y2)) {
                            possibleMoves.push_back({ x1, y1, x2, y2 });
                        }
                    }
                }
            }
        }
    }

    // wybierz losowy ruch i wykonaj go
    if (!possibleMoves.empty()) {
        int index = rand() % possibleMoves.size();
        Move chosen = possibleMoves[index];
        int points = board.moveBall(chosen.fromX, chosen.fromY, chosen.toX, chosen.toY);
        if (points >= 0) {
            addScore(points);
            std::cout << "Komputer przesunal kulke z (" << chosen.fromX << ", " << chosen.fromY
                << ") na (" << chosen.toX << ", " << chosen.toY << ")\n";
            if (points > 0)
                std::cout << "Komputer zdobyl " << points << " punktow. Lacznie: " << getScore() << "\n";
        }
    }
    else {
        std::cout << "Komputer nie mogl wykonac zadnego ruchu.\n";
    }
}


