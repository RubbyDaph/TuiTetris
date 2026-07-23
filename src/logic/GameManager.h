#pragma once

#include "Board.h"
#include "GameOverMenu.h"
#include "PausedMenu.h"
#include <iostream>
#include <random>
#include <chrono>

class Terminal;
class InputHandler;

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
    
    std::string GetGameFrame() const;
    std::string GetPausedFrame() const;
    std::string GetGameOverFrame() const;
    void Run(Terminal& terminal, InputHandler& input);
private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> figureDistribution;

    BoardClass board;
    PausedMenu pauseMenu;

    FigureType GenerateNextFigure();
    void Restart();
    void Tick();
    void HandleStepResult(const BoardStepResult& result);

    GameState gameState = GameState::Paused;
    unsigned int score{0};
    unsigned int lineCount{0};
};
