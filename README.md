# Kulki

A console logic game in C++, based on the classic *Color Lines* game. You move balls around a 9x9 board to build lines of 5 or more of the same colour.

I made it as a project for my Object-Oriented Programming course at UKSW in Warsaw (2025). The game text is in Polish.

## Rules

- Move one ball per turn by typing the start and target coordinates (`x1 y1 x2 y2`).
- A ball can only move if there is a free path to the target field.
- A line of at least 5 balls of the same colour (horizontal, vertical or diagonal) disappears and gives you points.
- After every move new random balls appear on the board.
- The game ends when the board is full.

## Features

- 3 game modes: solo, player vs computer, player vs player
- Computer opponent that makes its own moves
- Path finding, so a ball cannot jump over other balls
- The game saves automatically after every move and can be continued later
- Player ranking saved to a file
- Input validation (wrong format or coordinates outside the board are rejected)

## Example

```
   0 1 2 3 4 5 6 7 8
  +------------------+
0 |. . . . . . . . . |
1 |. . . . R . . . . |
2 |. . . . . . R . . |
3 |. . R . . . . . . |
4 |. . . R . . . . . |
5 |. . . . . . . . G |
6 |. . . . . . . . . |
7 |. . R . . . . . . |
8 |. . . R R R R . . |
  +------------------+
Punkty: 0
```

## How it works

| Class | What it does |
|---|---|
| `Game` | Main game loop, turns, saving and loading |
| `Board` | The board: moving balls, adding new ones, checking paths and removing full lines |
| `Player` | Abstract base class with the nickname and score |
| `HumanPlayer` | Reads the move from the keyboard |
| `ComputerPlayer` | Chooses a move on its own |

`HumanPlayer` and `ComputerPlayer` inherit from `Player` and override `makeMove()`. `Game` keeps both players as `std::unique_ptr<Player>`, so it does not need to know which type of player is making the move.

## How to run

With g++:

```
cd src
g++ -std=c++17 -o kulki *.cpp
./kulki
```

With Visual Studio: create an empty C++ console project and add all files from `src`.

## Author

Karolina Czekaj
