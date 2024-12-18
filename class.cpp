#include "class.h"

// Cell class implementation
Cell::Cell() : visited(false), topWall(true), bottomWall(true), leftWall(true), rightWall(true) {}

// Position class implementation
Position::Position(int px, int py) : x(px), y(py) {}

// Obstacle class implementation
Obstacle::Obstacle(int px, int py, int pdx, int pdy, Color pcolor)
    : x(px), y(py), dx(pdx), dy(pdy), color(pcolor) {}

void Obstacle::Draw() const {
    DrawRectangle(x * 40 + 5, y * 40 + 5, 30, 30, color);
}

void Obstacle::Update() {
    x += dx;
    y += dy;

    // Boundary check and reverse direction
    if (x < 0 || x >= 10) dx = -dx;
    if (y < 0 || y >= 7) dy = -dy;
}
