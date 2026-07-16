#include "Board.h"


BoardClass::BoardClass()
{
    this->Spawn(FigureType::I);
}

bool BoardClass::IsActiveTetrominoCell(int x, int y) const
{
    if(!hasActiveTetromino) return false;
    const Shape& shape = GetShape(activeTetromino.type, activeTetromino.direction);

    for(const Point& block : shape)
    {
        const int blockX = activeTetromino.position.x + block.x;
        const int blockY = activeTetromino.position.y + block.y;
        
        if(blockX == x && blockY == y) return true;
    }

    return false;
}

std::string BoardClass::Render() const
{
    std::string result;

    result += "┌────────────────────┐\n";
    for(int y = 0; y < height; y++)
    {
        result += "│";

        for(int x = 0; x < width; x++)
        {
            if (IsActiveTetrominoCell(x , y)) 
            {
                result += GetShapeColor(activeTetromino.type); 
            }
            else if(field[y][x] == 1)
            {
                result += GetLockedColor();
            }
            else 
            {
                result += "  ";
            }
        }
        result += "│\n";
    }
    result += "└────────────────────┘\n";

    return result;
}

bool BoardClass::CanPlace(const Tetromino& candidate) const
{
    // TODO: 
    // scale it up a bit, include case when block spawns above upper border
    // make exception for one of the checks
    const Shape& shape = GetShape(candidate.type, candidate.direction);

    for(const Point& block : shape)
    {
        const int blockX = candidate.position.x + block.x;
        const int blockY = candidate.position.y + block.y;
        
        const bool outsideBoard = blockX < 0 || blockX >= width || blockY < 0 || blockY >= height;

        if(outsideBoard) return false;

        if(field[blockY][blockX] != 0) return false;
    }
    return true;
}

bool BoardClass::Spawn(FigureType type)
{
    const int centerX = width / 2 - 2;
    
    Tetromino candidate = activeTetromino;

    candidate.type = type;
    candidate.position.x = centerX;
    candidate.position.y = 0;
    candidate.direction = TurnDirection::Up;

    if(CanPlace(candidate))
    {
        activeTetromino = candidate;
        hasActiveTetromino = true;
        return true;
    }
    return false;
}

void BoardClass::LockActiveTetromino()
{
    const Shape& shape = GetShape(activeTetromino.type, activeTetromino.direction);

    for(const auto& block : shape)
    {
        const int blockX = activeTetromino.position.x + block.x;
        const int blockY = activeTetromino.position.y + block.y;
        
        field[blockY][blockX] = 1;
    }
    hasActiveTetromino = false;
}

int BoardClass::ClearLines()
{
    int clearedLines = 0;

    for(int y = height - 1; y >= 0; y--)
    {
        bool lineIsFull = true;
        for(int x = 0; x < width; x++)
        {
            if(field[y][x] == 0)
            {
                lineIsFull = false;
                break;
            }
        }
        if(!lineIsFull) continue;

        for(int row = y; row > 0; --row)
        {
            for(int x = 0; x < width; ++x)
            {
                field[row][x] = field[row - 1][x];
            }
        }

        for(int x = 0; x < width; ++x)
        {
            field[0][x] = 0;
        }

        clearedLines++;
        y++;
    }
    return clearedLines;
}

BoardStepResult BoardClass::TryMoveDown() 
{
    Tetromino temp = activeTetromino;
    temp.position.y++;
    
    if(CanPlace(temp))
    {
        activeTetromino.position.y++;
        return BoardStepResult{0 , StepResult::Moved};
    }
    else
    {
        this->LockActiveTetromino();
        if(!this->Spawn(FigureType::I))
        {
            return BoardStepResult{0, StepResult::GameOver};
        }
        return BoardStepResult{this->ClearLines(), StepResult::Locked};
    }
}
