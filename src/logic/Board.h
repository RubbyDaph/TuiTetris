#pragma once

#include "Tetromino.h"
#include <optional>

enum class StepResult
{
    Moved,
    Locked,
    Blocked
};

struct BoardStepResult
{
    int clearedLines;
    StepResult stepResult;
};


enum class RotationMoveResult
{
    Rotated,
    Blocked
};



class BoardClass
{
public:
    BoardClass();
    ~BoardClass() = default;

    std::string Render(unsigned int score, const FigureType& next) const;
    BoardStepResult TryHardDrop();
    BoardStepResult TryMoveDown();
    StepResult TryMoveLeft();
    StepResult TryMoveRight();
    RotationMoveResult TryRotateClockwise();
    RotationMoveResult TryRotateCounterClockwise();
    std::optional<FigureType> HoldCurrentTetromino();
    bool Spawn(FigureType type);
    bool Reset(FigureType type);
    std::optional<BoardStepResult> TryLockGroundedTetromino();
    bool IsGrounded() const;
private:
    

    static constexpr int height = 20;
    static constexpr int width = 10;

    static constexpr int underWindowHeight = 6;


    bool hasActiveTetromino = true;

    Tetromino activeTetromino;
    std::optional<FigureType> heldTetromino;

    int field[height][width]{};

    bool IsActiveTetrominoCell(int x, int y ) const;
    bool IsTetrominoCell(const Tetromino& tetromino, int x, int y) const;
    std::optional<Tetromino> CalculateGhostTetromino() const;
    bool CanPlace(const Tetromino& candidate) const;
    void LockActiveTetromino();
    int ClearLines();


    bool CanTurn(const std::array<Point, 4>& tests, Tetromino& candidate);
    bool I_WallKickTests(TurnDirection nextDirection, Tetromino& candidate);
    bool Other_WallKickTests(TurnDirection nextDirection, Tetromino& candidate);
    std::string ScoreWindow(unsigned int score) const;
    std::string HoldWindow() const;
    std::string QueueWindow(const FigureType& nextTetromino) const;
};
