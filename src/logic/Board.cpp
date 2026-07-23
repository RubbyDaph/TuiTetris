#include "Board.h"
#include <algorithm>
#include <limits>
#include <string_view>

namespace
{
    void AppendSidebar(
        std::string& destination,
        std::string_view board,
        std::string_view sidebar)
    {
        std::size_t boardPosition = 0;
        std::size_t sidebarPosition = 0;

        while(boardPosition < board.size())
        {
            const std::size_t boardEnd = board.find('\n', boardPosition);
            const std::size_t boardLength =
                (boardEnd == std::string_view::npos ? board.size() : boardEnd) - boardPosition;

            destination.append(board.substr(boardPosition, boardLength));

            if(sidebarPosition < sidebar.size())
            {
                const std::size_t sidebarEnd = sidebar.find('\n', sidebarPosition);
                const std::size_t sidebarLength =
                    (sidebarEnd == std::string_view::npos ? sidebar.size() : sidebarEnd)
                    - sidebarPosition;

                destination += "  ";
                destination.append(sidebar.substr(sidebarPosition, sidebarLength));
                sidebarPosition =
                    sidebarEnd == std::string_view::npos ? sidebar.size() : sidebarEnd + 1;
            }

            destination += '\n';

            boardPosition = boardEnd == std::string_view::npos ? board.size() : boardEnd + 1;
        }
    }
}


BoardClass::BoardClass()
{
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


std::string BoardClass::ScoreWindow(unsigned int score) const
{
    std::string result;
    const std::string scoreText = std::to_string(score);
    const std::size_t leftPadding = (width - scoreText.size()) / 2;
    const std::size_t rightPadding = width - scoreText.size() - leftPadding;

    result += "┌──────────┐\n";
    result += "│  SCORE   │\n";
    result += "│" + std::string(leftPadding, ' ') + scoreText
           + std::string(rightPadding, ' ') + "│\n";
    result += "└──────────┘\n";

    return result;
}

std::string BoardClass::HoldWindow() const
{
    constexpr int previewWidth = 5;
    constexpr int previewHeight = 4;

    std::string result;
    result += "┌──────────┐\n";

    int offsetX = 0;
    int offsetY = 0;

    if(heldTetromino)
    {
        const Shape& shape = GetShape(*heldTetromino, TurnDirection::Up);

        int minX = std::numeric_limits<int>::max();
        int maxX = std::numeric_limits<int>::min();
        int minY = std::numeric_limits<int>::max();
        int maxY = std::numeric_limits<int>::min();

        for(const Point& block : shape)
        {
            minX = std::min(minX, block.x);
            maxX = std::max(maxX, block.x);
            minY = std::min(minY, block.y);
            maxY = std::max(maxY, block.y);
        }

        const int shapeWidth = maxX - minX + 1;
        const int shapeHeight = maxY - minY + 1;
        offsetX = (previewWidth - shapeWidth) / 2 - minX;
        offsetY = (previewHeight - shapeHeight) / 2 - minY;
    }

    for(int y = 0; y < underWindowHeight; ++y)
    {
        result += "│";

        if(y == 0)
        {
            result += "   HOLD   ";
        }
        else
        {
            const int previewY = y - 1;

            for(int x = 0; x < previewWidth; ++x)
            {
                bool isHeldCell = false;

                if(heldTetromino && previewY < previewHeight)
                {
                    const Shape& shape = GetShape(*heldTetromino, TurnDirection::Up);

                    for(const Point& block : shape)
                    {
                        if(block.x + offsetX == x && block.y + offsetY == previewY)
                        {
                            isHeldCell = true;
                            break;
                        }
                    }
                }

                result += isHeldCell ? GetShapeColor(*heldTetromino) : "  ";
            }
        }

        result += "│\n";
    }

    result += "└──────────┘\n";
    return result;
}

std::string BoardClass::Render(unsigned int score) const
{
    std::string boardWindow;

    boardWindow += "┌────────────────────┐\n";
    for(int y = 0; y < height; y++)
    {
        boardWindow += "│";

        for(int x = 0; x < width; x++)
        {
            if (IsActiveTetrominoCell(x , y)) 
            {
                boardWindow += GetShapeColor(activeTetromino.type);
            }
            else if(field[y][x] == 1)
            {
                boardWindow += GetLockedColor();
            }
            else 
            {
                boardWindow += "  ";
            }
        }
        boardWindow += "│\n";
    }
    boardWindow += "└────────────────────┘\n";

    std::string sidebar = ScoreWindow(score);
    sidebar += '\n';
    sidebar += HoldWindow();

    std::string result;
    AppendSidebar(result, boardWindow, sidebar);

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
    
    Tetromino candidate{type, TurnDirection::Up, {centerX, 0}};

    if(CanPlace(candidate))
    {
        activeTetromino = candidate;
        hasActiveTetromino = true;
        return true;
    }
    return false;
}

std::optional<FigureType> BoardClass::HoldCurrentTetromino()
{
    const FigureType currentType = activeTetromino.type;
    const std::optional<FigureType> previouslyHeld = heldTetromino;

    heldTetromino = currentType;
    hasActiveTetromino = false;

    return previouslyHeld;
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

BoardStepResult BoardClass::TryHardDrop()
{
    BoardStepResult result;
    while(true)
    {
       result = TryMoveDown();
       if(result.stepResult == StepResult::Locked) return result;
    }
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
        int clearedLines = this->ClearLines();
        return BoardStepResult{clearedLines, StepResult::Locked};
    }
}

bool BoardClass::Reset(FigureType type)
{
    heldTetromino.reset();
    hasActiveTetromino = false;

    for(int row = 0; row < this->height; row++)
    {
        for(int col = 0; col < this->width; col++)
        {
            field[row][col] = 0;
        }
    }
    
    return this->Spawn(type);
}

SideMoveResult BoardClass::TryMoveLeft()
{
    Tetromino candidate = activeTetromino;
    candidate.position.x--;

    if(CanPlace(candidate))
    {
        activeTetromino.position.x--;
        return SideMoveResult::Moved;
    }
    else
    {
        return SideMoveResult::Blocked;
    }
}

SideMoveResult BoardClass::TryMoveRight()
{
    Tetromino candidate = activeTetromino;
    candidate.position.x++;

    if(CanPlace(candidate))
    {
        activeTetromino.position.x++;
        return SideMoveResult::Moved;
    }
    else
    {
        return SideMoveResult::Blocked;
    }
}

bool BoardClass::CanTurn(const std::array<Point, 4>& tests, Tetromino& candidate)
{ 
    if(CanPlace(candidate)) return true;
    
    for(const Point& test : tests)
    {
        candidate.position.x += test.x;
        candidate.position.y += test.y;
        if(CanPlace(candidate)) return true;
    }
    return false;
}

bool BoardClass::I_WallKickTests(TurnDirection nextDirection, Tetromino& candidate)
{
    std::array<Point, 4> tests{};
    candidate.direction = nextDirection;
    switch(activeTetromino.direction)
    {
        case TurnDirection::Up:
        {
            if(nextDirection == TurnDirection::Right)
            {
                 tests = 
                 {
                    Point{-2, 0},
                    Point{+1, 0},
                    Point{-2, +1},
                    Point{+1, -2}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Left)
            {
                tests = 
                {
                    Point{-1, 0},
                    Point{+2, 0},
                    Point{-1, -2},
                    Point{+2, +1}
                };
                
                return CanTurn(tests, candidate);
            }
            break;
        }
        case TurnDirection::Right:
        {
            if(nextDirection == TurnDirection::Up)
            {
                tests =
                {
                    Point{+2, 0},
                    Point{-1, 0},
                    Point{+2, -1},
                    Point{-1, +2}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Down)
            {
                tests =
                {
                    Point{-1, 0},
                    Point{+2, 0},
                    Point{-1, -2},
                    Point{+2, +1}
                };
                return CanTurn(tests, candidate);
            }
            break;
        }
        case TurnDirection::Left:
        {
            if(nextDirection == TurnDirection::Up)
            {
                tests =
                {
                    Point{+1, 0},
                    Point{-2, 0},
                    Point{+1, +2},
                    Point{-2, -1}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Down)
            {
                tests =
                {
                    Point{-2, 0},
                    Point{+1, 0},
                    Point{-2, -1},
                    Point{+1, -2}
                };
                return CanTurn(tests, candidate);
            }
            break;
        }
        case TurnDirection::Down:
        {
            if(nextDirection == TurnDirection::Right)
            {
                tests =
                {
                    Point{+1, 0},
                    Point{-2, 0},
                    Point{+1, +2},
                    Point{-2, -1}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Left)
            {
                tests =
                {
                    Point{+2, 0},
                    Point{-1, 0},
                    Point{+2, -1},
                    Point{-1, +2}
                };
                return CanTurn(tests, candidate);
            }
            break;
        }
    }
    return false;
}

bool BoardClass::Other_WallKickTests(TurnDirection nextDirection, Tetromino& candidate)
{
    std::array<Point, 4> tests{};
    candidate.direction = nextDirection;
    switch(activeTetromino.direction)
    {
        case TurnDirection::Up:
        {
            if(nextDirection == TurnDirection::Right)
            {
                tests = {
                    Point{-1, 0},
                    Point{-1,-1},
                    Point{0, +2},
                    Point{-1, +2}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Left)
            {
                tests =
                {
                    Point{+1, 0},
                    Point{+1,-1},
                    Point{0, +2},
                    Point{+1, +2}
                };
                
                return CanTurn(tests, candidate);
            }
            break;
        }
        case TurnDirection::Right:
        {
            tests =
            {
                Point{+1, 0},
                Point{+1, +1},
                Point{0, -2},
                Point{+1, -2}
            };
            return CanTurn(tests, candidate);
        }
        case TurnDirection::Left:
        {
            tests =
            {
                Point{-1, 0},
                Point{-1, +1},
                Point{0, -2},
                Point{-1, -2}
            };
            return CanTurn(tests, candidate);
        }
        case TurnDirection::Down:
        {
            if(nextDirection == TurnDirection::Right)
            {
                tests =
                {
                    Point{-1,0},
                    Point{-1,-1},
                    Point{0, +2},
                    Point{-1, +2}
                };
                return CanTurn(tests, candidate);
            }
            else if(nextDirection == TurnDirection::Left)
            {
                tests =
                {
                    Point{+1,0},
                    Point{+1,-1},
                    Point{0, +2},
                    Point{+1, +2}
                };
                return CanTurn(tests, candidate);
            }
            break;
        }
    }
    return false;
}


RotationMoveResult BoardClass::TryRotateClockwise()
{
    Tetromino candidate = activeTetromino;
    
    bool isTypeI = (activeTetromino.type == FigureType::I) ? true : false; 
    bool result;

    if(activeTetromino.direction == TurnDirection::Left)
    {
        if(isTypeI)
        {
            result = I_WallKickTests(TurnDirection::Up, candidate);        
        }
        else
        {
            result = Other_WallKickTests(TurnDirection::Up, candidate);
        }
    }
    else
    {
            TurnDirection nextDirection = static_cast<TurnDirection>(static_cast<int>(activeTetromino.direction) + 1);
        if(isTypeI)
        {
            result = I_WallKickTests(nextDirection, candidate);
        }
        else
        {
            result = Other_WallKickTests(nextDirection, candidate);
        }
    }

    if(result)
    {
        activeTetromino = candidate;
        return RotationMoveResult::Rotated;
    }
    else
    {
        return RotationMoveResult::Blocked;
    }
}

RotationMoveResult BoardClass::TryRotateCounterClockwise()
{
    Tetromino candidate = activeTetromino;

    bool isTypeI = (activeTetromino.type == FigureType::I) ? true : false; 
    bool result;
    
    if(activeTetromino.direction == TurnDirection::Up)
    {
        if(isTypeI)
        {
            result = I_WallKickTests(TurnDirection::Left, candidate);        
        }
        else
        {
            result = Other_WallKickTests(TurnDirection::Left, candidate);
        }
    }
    else
    {
            TurnDirection nextDirection = static_cast<TurnDirection>(static_cast<int>(activeTetromino.direction) - 1);
        if(isTypeI)
        {
            result = I_WallKickTests(nextDirection, candidate);
        }
        else
        {
            result = Other_WallKickTests(nextDirection, candidate);
        }
    }

    if(result)
    {
        activeTetromino = candidate;
        return RotationMoveResult::Rotated;
    }
    else
    {
        return RotationMoveResult::Blocked;
    }

}
