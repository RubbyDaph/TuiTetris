#pragma once

#include "Board.h"
#include "GameOverMenu.h"
#include "PausedMenu.h"

#include <optional>
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
    std::size_t nextFigureIndex{0};
    std::array<FigureType, 7> figureBag;

    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    static constexpr auto lockDelay = std::chrono::milliseconds(1000);

    static constexpr unsigned int maxLockResets = 15;

    std::optional<TimePoint> lockDeadline;
    unsigned int lockResetCount{0};

    BoardClass board;
    PausedMenu pauseMenu;
    GameOverMenu gameOverMenu;

    FigureType TakeNextFigure();
    void Restart();
    void Tick(TimePoint now);
    void HandleStepResult(const BoardStepResult& result);
    void TryLockGroundedTetromino();
    void HandleDownResult(const BoardStepResult& result, TimePoint now);
    void RefreshLockDelayAfterMove(TimePoint now);
    void ResetLockState();

    GameState gameState = GameState::Paused;
    bool holdAvailable{true};
    unsigned int score{0};
};
