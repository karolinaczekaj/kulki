#include "Board.h"
#include <iostream>
#include <queue>
#include <random>
#include <algorithm>
#include <utility>
#include <ostream>

Board::Board(int w, int h) : width(w), height(h) {
    reset(); // start - czyscimy plansze
}

void Board::reset() {
    grid.assign(height, std::vector<Colour>(width, Colour::EMPTY)); // pusta plansza
    addRandomBalls(NEW_BALLS_PER_TURN); // wrzuc pare kulek na start
}

int Board::addRandomBalls(int n)
{
    std::vector<std::pair<int, int>> freeCells;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (grid[y][x] == Colour::EMPTY)
                freeCells.emplace_back(x, y); // zbieramy wolne pola

    int toAdd = std::min(n, static_cast<int>(freeCells.size()));
    if (toAdd == 0) return 0;

    static std::mt19937 rng{ std::random_device{}() };
    static const Colour palette[2]{ Colour::R,Colour::G};
    std::shuffle(freeCells.begin(), freeCells.end(), rng); // losowe miejsca
    std::uniform_int_distribution<int> col(0, 1); // losowy kolor

    for (int i = 0; i < toAdd; ++i) {
        auto [x, y] = freeCells[i];
        grid[y][x] = palette[col(rng)]; // wrzuc losowa kulke
    }

    return toAdd;
}

bool Board::pathExists(int x1, int y1, int x2, int y2) const {
    // sprawdzamy czy da sie dojsc z punktu A do B (bfs)
    if (!inside(x1, y1) || !inside(x2, y2) || grid[y1][x1] == Colour::EMPTY || grid[y2][x2] != Colour::EMPTY)
        return false;

    std::queue<std::pair<int, int>> q;
    q.emplace(x1, y1);

    std::vector<std::vector<char>> vis(height, std::vector<char>(width, 0));
    vis[y1][x1] = 1;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == x2 && y == y2) return true;
        for (auto [dx, dy] : dirs4) {
            int nx = x + dx, ny = y + dy;
            if (inside(nx, ny) && !vis[ny][nx] && grid[ny][nx] == Colour::EMPTY) {
                vis[ny][nx] = 1;
                q.emplace(nx, ny);
            }
        }
    }
    return false;
}

int Board::removeCompletedLines() {
    // szukamy linii 5+ takich samych kulek i je wywalamy
    std::vector<std::pair<int, int>> toErase;

    auto scan = [&](int dx, int dy) {
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                if (grid[y][x] == Colour::EMPTY) continue;
                int len = 1;
                while (inside(x + dx * len, y + dy * len) &&
                    grid[y + dy * len][x + dx * len] == grid[y][x])
                    ++len;
                if (len >= 5) {
                    for (int k = 0; k < len; ++k)
                        toErase.emplace_back(x + dx * k, y + dy * k);
                }
            }
        };

    scan(1, 0);  // poziomo
    scan(0, 1);  // pionowo
    scan(1, 1);  // skos 
    scan(1, -1); // skos 

    std::sort(toErase.begin(), toErase.end());
    toErase.erase(std::unique(toErase.begin(), toErase.end()), toErase.end());

    for (auto [x, y] : toErase)
        grid[y][x] = Colour::EMPTY;

    score += static_cast<int>(toErase.size());
    return static_cast<int>(toErase.size());
}

bool Board::boardFull() const {
    for (auto& r : grid)
        for (auto c : r)
            if (c == Colour::EMPTY) return false;
    return true;
}

void Board::display() const {
    std::cout << "\n   ";
    for (int x = 0; x < width; ++x) std::cout << x << ' ';
    std::cout << "\n  +" << std::string(width * 2, '-') << "+\n";

    for (int y = 0; y < height; ++y) {
        std::cout << y << " |";
        for (int x = 0; x < width; ++x)
            std::cout << static_cast<char>(grid[y][x]) << ' ';
        std::cout << "|\n";
    }

    std::cout << "  +" << std::string(width * 2, '-') << "+\n";
    std::cout << "Punkty: " << score << "\n\n"; 
}

int Board::moveBall(int x1, int y1, int x2, int y2) {
    if (!isFree(x2, y2) || grid[y1][x1] == Colour::EMPTY || !pathExists(x1, y1, x2, y2))
        return 0;

    std::swap(grid[y1][x1], grid[y2][x2]); // przesuwamy kulke

    int removed = removeCompletedLines();
    if (removed == 0) {
        int added = addRandomBalls(NEW_BALLS_PER_TURN);
        if (added > 0)
            removed = removeCompletedLines(); // moze cos sie teraz ulozylo
    }

    return removed;
}

Board::Colour Board::getSymbol(int x, int y) const {
    return grid[y][x];
}

void Board::setSymbol(int x, int y, Colour c) {
    grid[y][x] = c;
}

// zeby mozna bylo Colour wypisywac przez << jak zwykla litere
std::ostream& operator<<(std::ostream& os, Board::Colour c) {
    return os << static_cast<char>(c);
}

