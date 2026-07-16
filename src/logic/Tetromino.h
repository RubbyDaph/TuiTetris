#pragma once

#include <array>
#include <string>

enum class FigureType
{
    T,
    S,
    Z,
    J,
    L,
    O,
    I
};

enum class TurnDirection
{
    Up,
    Right,
    Down,
    Left
};

struct Point
{
    int x;
    int y;
};

using Shape = std::array<Point, 4>;

using FigureRotations = std::array<Shape, 4>;

struct Tetromino
{
    FigureType type;
    TurnDirection direction;
    Point position;
};

const Shape& GetShape(FigureType type, TurnDirection direction);

TurnDirection GetNextDirection(TurnDirection direction);

std::string GetShapeColor(FigureType type);

std::string GetLockedColor();
