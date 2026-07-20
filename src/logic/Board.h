#pragma once

#include "Tetromino.h"

enum class StepResult
{
    Moved,
    Locked
};

struct BoardStepResult
{
    int clearedLines;
    StepResult stepResult;
};

enum class SideMoveResult
{
    Moved,
    Blocked
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

    std::string Render() const;
    BoardStepResult TryMoveDown();
    SideMoveResult TryMoveLeft();
    SideMoveResult TryMoveRight();
    RotationMoveResult TryRotateClockwise();
    RotationMoveResult TryRotateCounterClockwise();
    bool Spawn(FigureType type);
    bool Reset(FigureType type);
private:
    

    static constexpr int height = 20;
    static constexpr int width = 10;

    bool hasActiveTetromino = true;

    Tetromino activeTetromino;

    int field[height][width]{};

    bool IsActiveTetrominoCell(int x, int y ) const;
    bool CanPlace(const Tetromino& candidate) const;
    void LockActiveTetromino();
    int ClearLines();


    bool CanTurn(const std::array<Point, 4>& tests, Tetromino& candidate);
    bool I_WallKickTests(TurnDirection nextDirection, Tetromino& candidate);
    bool Other_WallKickTests(TurnDirection nextDirection, Tetromino& candidate);
};
