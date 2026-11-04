#pragma once
#include <vector>
#include <string>
#include <ostream>  // Potrzebne do operatora <<

// Klasa reprezentująca planszę do gry "Kulki"
// Odpowiada za wyświetlanie, przemieszczanie kulek, dodawanie nowych,
// usuwanie linii oraz śledzenie punktów i stanu planszy.

class Board {
public:
    // Możliwe kolory kulek. Każdy kolor zapisany jako pojedynczy znak.
    // EMPTY oznacza puste pole (kropka).
    enum class Colour {
        EMPTY = '.', R = 'R', G = 'G'
    };

private:
    std::vector<std::vector<Colour>> grid;  // Siatka przechowująca stan planszy
    int width, height;                      // Wymiary planszy
    static constexpr int NEW_BALLS_PER_TURN = 10;  // Ile kulek dodajemy po każdym ruchu
    int score = 0;                          // Aktualny wynik gracza

    // Pomocnicza funkcja sprawdzająca, czy współrzędne są w granicach planszy
    bool inside(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }

    // Kierunki sąsiednich pól w 4 strony (góra, dół, lewo, prawo)
    static inline const std::vector<std::pair<int, int>> dirs4{ {1,0},{-1,0},{0,1},{0,-1} };

    // Kierunki w 8 stron (łącznie z ukosami) – do sprawdzania linii
    static inline const std::vector<std::pair<int, int>> dirs8{ {1,0},{-1,0},{0,1},{0,-1},
                                                                {1,1},{1,-1},{-1,1},{-1,-1} };
    std::vector<std::vector<char>> previousState;


public:
    // Konstruktor – tworzy planszę o podanej szerokości i wysokości
    Board(int w, int h);

    // Czyści planszę do stanu początkowego
    void reset();

    // Wyświetla planszę na ekranie
    void display() const;

    // Próbuje przenieść kulkę z (x1, y1) do (x2, y2)
    // Zwraca liczbę zdobytych punktów lub 0, jeśli ruch był nieudany
    int moveBall(int x1, int y1, int x2, int y2);

    // Sprawdza, czy pole (x, y) jest puste
    bool isFree(int x, int y) const { return grid[y][x] == Colour::EMPTY; }

    // Zwraca kulkę (kolor) na danym polu
    Colour getSymbol(int x, int y) const;

    // Ustawia kulkę (kolor) na danym polu
    void setSymbol(int x, int y, Colour c);

    // Dodaje n losowych kulek na wolne miejsca planszy
    // Zwraca liczbę dodanych kulek
    int addRandomBalls(int n);

    // Sprawdza, czy można przejść z (x1, y1) do (x2, y2)
    // (czyli czy istnieje ścieżka pustych pól)
    bool pathExists(int x1, int y1, int x2, int y2) const;

    // Usuwa wszystkie kompletne linie (min. 5 kulek tego samego koloru)
    // Zwraca liczbę zdobytych punktów
    int removeCompletedLines();

    // Zwraca aktualny wynik gracza
    int getScore() const { return score; }

    // Sprawdza, czy cała plansza jest zapełniona
    bool boardFull() const;

  
};

// Operator << pozwala łatwo wypisywać kulki (kolory) jako znaki
std::ostream& operator<<(std::ostream& os, Board::Colour c);
