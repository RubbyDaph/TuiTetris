#pragma once

#include "Tetromino.h"

enum class StepResult
{
    Moved,
    Locked,
    GameOver
};

struct BoardStepResult
{
    int clearedLines;
    StepResult stepResult;
};


class BoardClass
{
public:
    BoardClass();
    ~BoardClass() = default;

    std::string Render() const;
    BoardStepResult TryMoveDown();
    
private:
    static constexpr int height = 20;
    static constexpr int width = 10;
    bool hasActiveTetromino = true;
    Tetromino activeTetromino;
    int field[height][width]{};
    bool IsActiveTetrominoCell(int x, int y ) const;
    bool CanPlace(const Tetromino& candidate) const;
    bool Spawn(FigureType type);
    void LockActiveTetromino();
    int ClearLines();
};
