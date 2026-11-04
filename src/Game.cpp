#include "Game.h"
#include "Board.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include <iostream>
#include <fstream>
//RANKING SORTOWANIE I WYPISYWANIE
#include <vector>
#include <algorithm>
#include <sstream>

void pokazRanking() {
    std::ifstream file("ranking.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc ranking.txt\n";
        return;
    }

    // Struktura do przechowywania danych graczy
    struct Gracz {
        std::string name;
        int score;
    };

    std::vector<Gracz> ranking;
    std::string line;

    // Wczytywanie danych z pliku
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (iss >> name >> score) {
            ranking.push_back({ name, score });
        }
    }

    file.close();

    // Sortowanie malejąco po wyniku
    std::sort(ranking.begin(), ranking.end(), [](const Gracz& a, const Gracz& b) {
        return a.score > b.score;
        });

    // Wypisywanie posortowanego rankingu
    std::cout << "\n=== RANKING ===\n";
    for (size_t i = 0; i < ranking.size(); ++i) {
        std::cout << i + 1 << ". " << ranking[i].name << " - " << ranking[i].score << " pkt\n";
    }
}


Game::Game() : board(9, 9), isPlayer1Turn(true) {}

bool Game::loadSavedGame() {
    std::ifstream file("save.txt");
    if (!file) return false;

    int score;
    file >> score;
    file.ignore();

    std::getline(file, player1Name);
    std::getline(file, player2Name);

    // odczyt stanu planszy
    for (int y = 0; y < 9; ++y) {
        std::string line;
        std::getline(file, line);
        for (int x = 0; x < 9; ++x) {
            board.setSymbol(x, y, static_cast<Board::Colour>(line[x]));
        }
    }

    int turnFlag;
    file >> turnFlag;
    isPlayer1Turn = (turnFlag == 1);

    return true;
}

void Game::saveGameState() {
    std::ofstream save("save.txt");
    save << board.getScore() << "\n";
    save << player1->getName() << "\n" << (player2 ? player2->getName() : "Brak") << "\n";

    // zapis planszy do pliku
    for (int y = 0; y < 9; ++y) {
        for (int x = 0; x < 9; ++x) {
            save << static_cast<char>(board.getSymbol(x, y));
        }
        save << '\n';
    }

    save << (isPlayer1Turn ? 1 : 0) << "\n";
    save.close();
}

void Game::run() {
    std::cout << "ZASADY GRY\n";
    std::cout << "Celem gry jest przesuwanie liter tak, aby tworzyc linie\n";
    std::cout << "z co najmniej 5 liter takich samych – w pionie,\n";
    std::cout << "poziomie lub na skos. Za kazda taka linie zdobywasz punkty.\n\n";

    int liczbaRuchow = 0;

    std::cout << "1. Nowa gra\n2. Kontynuuj\n";
    int choice;
    std::cin >> choice;

    bool kontynuuj = (choice == 2 && loadSavedGame());
    if (kontynuuj) {
        std::cout << "Wczytano zapis gry.\n";
        std::cout << "Gracz 1: " << player1Name << "\n";
        std::cout << "Gracz 2: " << player2Name << "\n";

        player1 = std::make_unique<HumanPlayer>(player1Name);
        if (player2Name == "Komputer")
            player2 = std::make_unique<ComputerPlayer>("Komputer");
        else if (player2Name == "Brak")
            player2 = nullptr;
        else
            player2 = std::make_unique<HumanPlayer>(player2Name);
    }
    else {
        board.reset();
        isPlayer1Turn = true;

        std::cout << "Podaj nick Gracza 1: ";
        std::cin >> player1Name;
        std::cout << "Podaj nick Gracza 2 (lub wpisz Komputer): ";
        std::cin >> player2Name;

        std::cout << "Tryb gry:\n1. Solo (1 gracz)\n2. Gracz vs Komputer\n3. Gracz vs Gracz\n";
        std::cin >> choice;

        if (choice == 1) {
            player1 = std::make_unique<HumanPlayer>(player1Name);
            player2 = nullptr;
        }
        else {
            player1 = std::make_unique<HumanPlayer>(player1Name);
            if (player2Name == "Komputer")
                player2 = std::make_unique<ComputerPlayer>("Komputer");
            else
                player2 = std::make_unique<HumanPlayer>(player2Name);
        }
    }

    // główna pętla gry
    while (true) {
        board.display();

        if (isPlayer1Turn || !player2) {
            std::cout << "Ruch: " << player1->getName() << "\n";
            player1->makeMove(board);
            isPlayer1Turn = false;
        }
        else {
            std::cout << "Ruch: " << player2->getName() << "\n";
            player2->makeMove(board);
            isPlayer1Turn = true;
        }
		//LIMIT LICZBY RUCHÓW 
        liczbaRuchow++;
        if (liczbaRuchow >= 30) {
            std::cout << "Osiągnięto limit 30 ruchów. Gra się kończy." << std::endl;
            break;
        }

        std::cout << player1->getName() << " ma teraz " << player1->getScore() << " punktow.\n";
        if (player2)
            std::cout << player2->getName() << " ma teraz " << player2->getScore() << " punktow.\n";

        saveGameState();

        if (board.boardFull()) {
            board.display();
            std::cout << "Koniec gry! Plansza jest pelna.\n";
            break;
        }
    }

    // wyniki końcowe
    std::cout << "Gra zakonczona!\n";
    std::cout << player1->getName() << " zdobyl(a): " << player1->getScore() << " punktow\n";
    // ZAPIS DO RANKINGU
    std::ofstream rankingFile("ranking.txt", std::ios::app);
    if (rankingFile.is_open()) {
        rankingFile << player1->getName() << " " << player1->getScore() << "\n";
        if (player2) {
            rankingFile << player2->getName() << " " << player2->getScore() << "\n";
        }
        rankingFile.close();
        std::cout << "Wyniki zapisane do rankingu.\n";
    }
    else {
        std::cerr << "Nie udalo sie zapisac do pliku ranking.txt.\n";
    }

   
    //cd.
    if (player2) {
        std::cout << player2->getName() << " zdobyl(a): " << player2->getScore() << " punktow\n";

        if (player1->getScore() > player2->getScore())
            std::cout << "Wygral(a): " << player1->getName() << "!\n";
        else if (player2->getScore() > player1->getScore())
            std::cout << "Wygral(a): " << player2->getName() << "!\n";
        else
            std::cout << "Remis!\n";
    }
    else {
        std::cout << "Tryb solo – Twoj wynik to: " << player1->getScore() << " punktow\n";
    }

    // czyszczenie zapisu gry
    if (std::remove("save.txt") == 0) {
        std::cout << "Zapis gry zostal usuniety.\n";
    }
    else {
        std::cerr << "Nie udalo sie usunac zapisu gry.\n";
    }
    pokazRanking();

}
