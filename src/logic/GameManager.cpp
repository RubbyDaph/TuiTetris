#include "GameManager.h"
#include "../terminal/Terminal.h"
#include "../terminal/InputHandler.h"

#include <algorithm>

GameManager::GameManager() : generator(std::random_device{}())
{
    using enum FigureType;
    figureBag = {T, S, Z, J, L, O, I};
    std::shuffle(figureBag.begin(), figureBag.end(), generator);
    if(!board.Reset(TakeNextFigure()))
    {
        throw std::runtime_error("Figure couldn't spawn");
    }
}

GameManager::~GameManager()
{
}

std::string GameManager::GetGameFrame()  const
{
    return board.Render(score, figureBag[nextFigureIndex]);
}

std::string GameManager::GetPausedFrame() const
{
    return pauseMenu.Render();
}

std::string GameManager::GetGameOverFrame() const
{
    return gameOverMenu.Render(this->score); 
}

FigureType GameManager::TakeNextFigure()
{
    FigureType type = figureBag[nextFigureIndex];
    nextFigureIndex++;
    if(nextFigureIndex >= figureBag.size())
    {
        std::shuffle(figureBag.begin(), figureBag.end(), generator);
        nextFigureIndex = 0;
    }
    return type;
}

void GameManager::HandleStepResult(const BoardStepResult& result)
{
    if(result.stepResult != StepResult::Locked) return;

    ResetLockState();

    score += result.clearedLines * (int)ScoreValue::ClearedLine;
    score += (int)ScoreValue::PlacedBlock;

    if(!board.Spawn(TakeNextFigure()))
    {
        gameState = GameState::GameOver;
    }
    else
    {
        holdAvailable = true;
    }
}

void GameManager::Tick(TimePoint now)
{
    if(gameState != GameState::Running) return;

    const BoardStepResult result = board.TryMoveDown();
    HandleDownResult(result, now);
}

void GameManager::Restart()
{
    score = 0;
    holdAvailable = true;
    ResetLockState();
    board.Reset(this->TakeNextFigure());
    pauseMenu.Reset();
    gameOverMenu.Reset();
    gameState = GameState::Running;
}

void GameManager::Run(Terminal& terminal, InputHandler& input)
{
    auto nextTick = std::chrono::steady_clock::now() + std::chrono::milliseconds(500); 
    while(gameState != GameState::Quit)
    {
        switch(gameState)
        {
            case GameState::Running:
                {
                    auto key = input.GetKey(16);
                    if(key)
                    {
                        switch(*key)
                        {
                            case Key::ArrowLeft:
                                {
                                    if(board.TryMoveLeft() == StepResult::Moved)
                                    {
                                        RefreshLockDelayAfterMove(Clock::now());
                                    }
                                    break;
                                }
                            case Key::ArrowRight:
                                {
                                    if(board.TryMoveRight() == StepResult::Moved)
                                    {
                                        RefreshLockDelayAfterMove(Clock::now());
                                    }
                                    break;
                                }
                            case Key::ArrowDown:
                                {
                                    const TimePoint now = Clock::now();
                                    HandleDownResult(board.TryMoveDown(), now);
                                    break;
                                }
                            case Key::A:
                                {
                                    if(board.TryRotateCounterClockwise()
                                       == RotationMoveResult::Rotated)
                                    {
                                        RefreshLockDelayAfterMove(Clock::now());
                                    }
                                    break;
                                }
                            case Key::D:
                                {
                                    if(board.TryRotateClockwise()
                                       == RotationMoveResult::Rotated)
                                    {
                                        RefreshLockDelayAfterMove(Clock::now());
                                    }
                                    break;
                                }
                            case Key::Escape:
                                {
                                    gameState = GameState::Paused;
                                    break;
                                }
                            case Key::P:
                                {
                                    gameState = GameState::Paused;
                                    break;
                                }
                            case Key::Q:
                                {
                                    gameState = GameState::Quit;
                                    break;
                                }
                            case Key::S:
                                {
                                    if(!holdAvailable)
                                    {
                                        break;
                                    }

                                    const std::optional<FigureType> previouslyHeld =
                                        board.HoldCurrentTetromino();
                                    const FigureType nextType =
                                        previouslyHeld ? *previouslyHeld : TakeNextFigure();

                                    holdAvailable = false;

                                    if(!board.Spawn(nextType))
                                    {
                                        gameState = GameState::GameOver;
                                    }

                                    ResetLockState();
                                    nextTick = Clock::now()
                                             + std::chrono::milliseconds(500);

                                    break;
                                }
                            case Key::Space:
                                {
                                    ResetLockState();
                                    HandleStepResult(board.TryHardDrop());
                                    nextTick = Clock::now()
                                             + std::chrono::milliseconds(500);
                                    break;
                                }
                            case Key::Other:
                                {
                                    break;
                                }
                            default: break;
                        }
                    }

                    auto now = Clock::now();

                    if(gameState == GameState::Running && now >= nextTick)
                    {
                        Tick(now);
                        nextTick += std::chrono::milliseconds(500);
                    }

                    now = Clock::now();

                    if(gameState == GameState::Running
                       && lockDeadline
                       && now >= *lockDeadline)
                    {
                        TryLockGroundedTetromino();
                    }

                    terminal.Present(this->GetGameFrame());
                    break;
                }
            case GameState::Paused:
                {
                    auto now = std::chrono::steady_clock::now();
                    if(now >= nextTick) nextTick += std::chrono::milliseconds(500);
                    auto key = input.GetKey(16);
                    if(key)
                    {
                        switch(*key)
                        {
                            case Key::ArrowDown:
                                {
                                    pauseMenu.GoNextOption();
                                    break;
                                }
                            case Key::ArrowUp:
                                {
                                    pauseMenu.GoPrevOption();
                                    break;
                                }
                            case Key::Q:
                                {
                                    gameState = GameState::Quit;
                                    break;
                                }
                            case Key::Enter:
                                {
                                    PausedMenuOption option = pauseMenu.ConfirmOption();
                                    switch(option)
                                    {
                                        case PausedMenuOption::Resume:
                                            {
                                                gameState = GameState::Running;
                                                const TimePoint now = Clock::now();
                                                nextTick = now + std::chrono::milliseconds(500);

                                                if(board.IsGrounded())
                                                {
                                                    lockDeadline = now + lockDelay;
                                                }
                                                else
                                                {
                                                    lockDeadline.reset();
                                                }
                                                break;
                                            }
                                        case PausedMenuOption::Restart:
                                            {
                                                this->Restart();
                                                nextTick = Clock::now()
                                                         + std::chrono::milliseconds(500);
                                                break;
                                            }
                                        case PausedMenuOption::Quit:
                                            {
                                                gameState = GameState::Quit;
                                                break;
                                            }
                                    }
                                    break;
                                }
                            case Key::Escape:
                                {
                                    gameState = GameState::Running;
                                    const TimePoint now = Clock::now();
                                    nextTick = now + std::chrono::milliseconds(500);

                                    if(board.IsGrounded())
                                    {
                                        lockDeadline = now + lockDelay;
                                    }
                                    else
                                    {
                                        lockDeadline.reset();
                                    }
                                    break;
                                }
                            default: break;
                            case Key::Other: break;
                        }
                    }
                    terminal.Present(this->GetPausedFrame());
                    break;
                }
            case GameState::GameOver:
                {
                    auto now = std::chrono::steady_clock::now();
                    if(now >= nextTick) nextTick += std::chrono::milliseconds(500);

                    auto key = input.GetKey(16);
                    if(key)
                    {
                        switch(*key)
                        {
                            case Key::ArrowDown:
                                {
                                    gameOverMenu.GoNextOption();
                                    break;
                                }
                            case Key::ArrowUp:
                                {
                                    gameOverMenu.GoPrevOption();
                                    break;
                                }
                            case Key::Enter:
                                {
                                    GameOverMenuOption option = gameOverMenu.ConfirmOption();
                                    switch(option)
                                    {
                                        case GameOverMenuOption::Restart:
                                            {
                                                this->Restart();
                                                nextTick = Clock::now()
                                                         + std::chrono::milliseconds(500);
                                                break;
                                            }
                                        case GameOverMenuOption::Quit:
                                            {
                                                gameState = GameState::Quit;
                                                break;
                                            }
                                    }
                                    break;
                                }
                            default: break;
                            case Key::Other: break;
                        }
                    }
                    terminal.Present(this->GetGameOverFrame());
                    break;
                }
            case GameState::Quit:
                {
                    return;
                }
        }

    }
}

void GameManager::TryLockGroundedTetromino()
{
    auto result = board.TryLockGroundedTetromino();

    lockDeadline.reset();

    if(!result) return;

    lockResetCount = 0;
    HandleStepResult(*result);
}


void GameManager::HandleDownResult(const BoardStepResult& result, TimePoint now)
{
    if(result.stepResult == StepResult::Moved)
    {
        if(board.IsGrounded())
        {
            if(!lockDeadline) lockDeadline = now + lockDelay;
        }
        else
        {
            lockDeadline.reset();
        }

        return;
    }

    if(result.stepResult == StepResult::Blocked)
    {
        if(!lockDeadline) lockDeadline = now + lockDelay;
    }
}

void GameManager::RefreshLockDelayAfterMove(TimePoint now)
{
    if(!board.IsGrounded())
    {
        lockDeadline.reset();
        return;
    }

    if(!lockDeadline)
    {
        lockDeadline = now + lockDelay;
        return;
    }

    if(lockResetCount < maxLockResets)
    {
        lockDeadline = now + lockDelay;
        ++lockResetCount;
    }
}

void GameManager::ResetLockState()
{
    lockDeadline.reset();
    lockResetCount = 0;
}
