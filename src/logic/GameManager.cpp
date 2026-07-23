#include "GameManager.h"
#include "../terminal/Terminal.h"
#include "../terminal/InputHandler.h"

GameManager::GameManager() : generator(std::random_device{}()), figureDistribution(0, 6)
{
    if(!board.Reset(GenerateNextFigure()))
    {
        throw std::runtime_error("Figure couldn't spawn");
    }
}

GameManager::~GameManager()
{
}

std::string GameManager::GetGameFrame()  const
{
    return board.Render();
}

std::string GameManager::GetPausedFrame() const
{
    return pauseMenu.Render();
}

std::string GameManager::GetGameOverFrame() const
{

}

FigureType GameManager::GenerateNextFigure()
{
    const int value = figureDistribution(generator);

    return static_cast<FigureType>(value);
}

void GameManager::HandleStepResult(const BoardStepResult& result)
{
    if(result.stepResult == StepResult::Moved) return;

    lineCount += result.clearedLines;
    score += result.clearedLines * (int)ScoreValue::ClearedLine;
    score += (int)ScoreValue::PlacedBlock;

    if(!board.Spawn(GenerateNextFigure()))
    {
        gameState = GameState::GameOver;
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
    lineCount = 0;
    board.Reset(this->GenerateNextFigure());
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
                                    // TODO: for hold, in the future

                                    break;
                                }
                            case Key::Space:
                                {
                                    // TODO: hard drop
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
                    // TODO: make gameover menu and input
                }
            case GameState::Quit:
                {
                    return;
                }
        }

    }
}
