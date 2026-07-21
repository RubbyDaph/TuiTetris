#include "Tetromino.h"

namespace 
{
    using FigureCatalog = std::array<FigureRotations, 7>;
    
    using ColorCatalog = std::array<std::string, 7>;

    const ColorCatalog colorCatalog
    {
        // T
        "\x1b[48;2;255;140;0m  \x1b[0m", // orange
        // S
        "\x1b[48;2;255;96;82m  \x1b[0m", // red
        // Z
        "\x1b[48;2;82;82;255m  \x1b[0m", // blue
        // J
        "\x1b[48;2;80;204;82m  \x1b[0m", // green
        // L
        "\x1b[48;2;252;247;96m  \x1b[0m", // yellow
        // O
        "\x1b[48;2;242;96;252m  \x1b[0m", // purple
        // I
        "\x1b[48;2;96;252;247m  \x1b[0m", // cyan
    };

    const std::string lockedColor =
        "\x1b[48;2;156;156;156m  \x1b[0m"; // grey

    constexpr FigureCatalog figureCatalog{
        // T
        FigureRotations{
            Shape{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
            Shape{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
            Shape{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}}
        },

        // S
        FigureRotations{
            Shape{Point{1, 0}, Point{2, 0}, Point{0, 1}, Point{1, 1}},
            Shape{Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
            Shape{Point{1, 1}, Point{2, 1}, Point{0, 2}, Point{1, 2}},
            Shape{Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}}
        },

        // Z
        FigureRotations{
            Shape{Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{2, 1}},
            Shape{Point{2, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
            Shape{Point{0, 1}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
            Shape{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2}}
        },

        // J
        FigureRotations{
            Shape{Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{1, 2}},
            Shape{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
            Shape{Point{1, 0}, Point{1, 1}, Point{0, 2}, Point{1, 2}}
        },

        // L
        FigureRotations{
            Shape{Point{2, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
            Shape{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{0, 2}},
            Shape{Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{1, 2}}
        },

        // O
        FigureRotations{
            Shape{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
            Shape{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}}
        },

        // I
        FigureRotations{
            Shape{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{3, 1}},
            Shape{Point{2, 0}, Point{2, 1}, Point{2, 2}, Point{2, 3}},
            Shape{Point{0, 2}, Point{1, 2}, Point{2, 2}, Point{3, 2}},
            Shape{Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{1, 3}}
        },
    };
}



const Shape& GetShape(FigureType type, TurnDirection direction)
{
    const auto figureIndex = static_cast<std::size_t>(type);
    const auto directionIndex = static_cast<std::size_t>(direction);

    return figureCatalog[figureIndex][directionIndex];
}

TurnDirection GetNextDirection(TurnDirection direction)
{
    switch(direction)
    {
        case TurnDirection::Up: return TurnDirection::Right;
        case TurnDirection::Right: return TurnDirection::Down;
        case TurnDirection::Down: return TurnDirection::Left;
        case TurnDirection::Left: return TurnDirection::Up;
    }

    return TurnDirection::Up;
}

std::string GetShapeColor(FigureType type)
{
    const auto figureIndex = static_cast<std::size_t>(type);

    return colorCatalog[figureIndex];
}

std::string GetLockedColor()
{
    return lockedColor;
}
