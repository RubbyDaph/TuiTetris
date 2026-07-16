#pragma once

#include "Board.h"

enum class GameState
{
    Running,
    Paused,
    GameOver,
    Quit
};

class GameManager
{
    BoardClass board;
public:
    GameManager();
    ~GameManager() = default;

    std::string GetFrame() const;
};
