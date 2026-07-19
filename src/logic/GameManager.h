#pragma once

#include "Board.h"
#include <random>

enum class GameState
{
    Running,
    Paused,
    GameOver,
    Quit
};

enum class ScoreValue
{
    PlacedBlock = 50,
    ClearedLine = 100
};

class GameManager
{
    BoardClass* board;
public:
    GameManager();
    ~GameManager();
    
    std::string GetFrame() const;
    void Tick();
private:
    FigureType GenerateNextFigure() const;
    GameState gameState = GameState::Running;
    unsigned int score;
    unsigned int lineCount;
};
