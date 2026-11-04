#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& n) : Player(n) {}

void HumanPlayer::makeMove(Board& board) {
    int x1, y1, x2, y2;

    while (true) {
        std::cout << name << ", podaj wsp. ruchu (x1 y1 x2 y2): ";

        // sprawdzamy czy dane sa poprawne
        if (!(std::cin >> x1 >> y1 >> x2 >> y2)) {
            std::cout << "Bledny format danych!\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        // czy wszystko miesci sie na planszy 9x9
        if (x1 < 0 || x1 >= 9 || y1 < 0 || y1 >= 9 || x2 < 0 || x2 >= 9 || y2 < 0 || y2 >= 9) {
            std::cout << "Wspolrzedne poza zakresem! Sprobuj ponownie.\n";
            continue;
        }

        // sprawdzamy czy na starcie jest kulka
        if (board.isFree(x1, y1)) {
            std::cout << "Na polu startowym nie ma kulki!\n";
            continue;
        }

        // i czy pole docelowe jest puste
        if (!board.isFree(x2, y2)) {
            std::cout << "Pole docelowe jest zajete!\n";
            continue;
        }

        // proba wykonania ruchu
        int points = board.moveBall(x1, y1, x2, y2);
        if (points < 0) {
            std::cout << "Niepoprawny ruch!\n";
            continue;
        }

        // aktualizacja punktow
        addScore(points);
        if (points > 0)
            std::cout << name << " zdobyl(a) " << points << " punktow! Lacznie: " << getScore() << "\n";
        else
            std::cout << "Brak punktow w tym ruchu.\n";

        break; // ruch wykonany – konczymy petle
    }
}
