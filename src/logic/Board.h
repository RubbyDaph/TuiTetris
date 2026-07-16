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


class BoardClass
{
public:
    BoardClass(FigureType firstFigure);
    ~BoardClass() = default;

    std::string Render() const;
    BoardStepResult TryMoveDown();
    bool Spawn(FigureType type);
    int getYep() {return (int)activeTetromino.type;}
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
};
