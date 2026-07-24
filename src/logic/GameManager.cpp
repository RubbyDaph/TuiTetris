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
    return board.Render(score);
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
    if(result.stepResult == StepResult::Moved) return;

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

void GameManager::Tick() 
{
    if(gameState != GameState::Running) return;

    BoardStepResult result = board.TryMoveDown();
    HandleStepResult(result);
}

void GameManager::Restart()
{
    score = 0;
    holdAvailable = true;
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
                                    board.TryMoveLeft();
                                    break;
                                }
                            case Key::ArrowRight:
                                {
                                    board.TryMoveRight();
                                    break;
                                }
                            case Key::ArrowDown:
                                {
                                    HandleStepResult(board.TryMoveDown());
                                    break;
                                }
                            case Key::A:
                                {
                                    board.TryRotateCounterClockwise();
                                    break;
                                }
                            case Key::D:
                                {
                                    board.TryRotateClockwise();
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

                                    nextTick = std::chrono::steady_clock::now()
                                             + std::chrono::milliseconds(500);

                                    break;
                                }
                            case Key::Space:
                                {
                                    HandleStepResult(board.TryHardDrop());
                                    break;
                                }
                            case Key::Other:
                                {
                                    break;
                                }
                            default: break;
                        }
                    }

                    auto now = std::chrono::steady_clock::now();

                    if(now >= nextTick)
                    {
                        Tick();
                        nextTick += std::chrono::milliseconds(500);
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
                                                break;
                                            }
                                        case PausedMenuOption::Restart:
                                            {
                                                this->Restart();
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
