#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <raylib.h>

// Enum for game state
enum GameState {
    MENU,
    PLAYING,
    WIN,
    LEVEL_COMPLETE,
    GAMEOVER
};

// Enum for difficulty levels
enum Level {
    FACILE,
    MOYEN,
    DIFFICULTE
};

// Cell class representing each maze cell
class Cell {
public:
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;

    Cell();
};

// Position class for coordinates
class Position {
public:
    int x, y;

    Position(int px = 0, int py = 0);
};

// Obstacle class for movable obstacles
class Obstacle {
public:
    int x, y;
    int dx, dy;
    Color color;

    Obstacle(int px, int py, int pdx, int pdy, Color pcolor);
    void Draw() const;
    void Update();
};

#endif
