#pragma once

#include "Board.h"
#include <random>
#include <iostream>

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
public:
    GameManager();
    ~GameManager();
    
    std::string GetFrame() const;
    void Tick();
private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> figureDistribution;

    BoardClass board;

    FigureType GenerateNextFigure();

    GameState gameState = GameState::Running;
    unsigned int score{0};
    unsigned int lineCount{0};
};
