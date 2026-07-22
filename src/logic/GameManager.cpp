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

void GameManager::Tick() 
{
    if(gameState != GameState::Running) return;

    BoardStepResult boardStepResult = board.TryMoveDown();

    if(boardStepResult.stepResult == StepResult::Locked)
    {
        lineCount += boardStepResult.clearedLines;
        score += boardStepResult.clearedLines * (int)ScoreValue::ClearedLine;
        score += (int)ScoreValue::PlacedBlock;
        if(!board.Spawn(this->GenerateNextFigure()))
        {
            gameState = GameState::GameOver;
        }
    }
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
                                // TODO: add soft drop mechanic
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
                        case Key::Other:
                            {
                                break;
                            }
                        default: break;
                    }
                }
                // TODO: add timer
                this->Tick();
                terminal.Present(this->GetGameFrame());
                break;
            }
            case GameState::Paused:
            {
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
                        case Key::Enter:
                        {
                            MenuOption option = pauseMenu.ConfirmOption();
                            switch(option)
                            {
                                case MenuOption::Resume:
                                {
                                    gameState = GameState::Running;
                                    break;
                                }
                                case MenuOption::Restart:
                                {
                                    break;
                                }
                                case MenuOption::Quit:
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
                // TODO: make gameover menu and input
            }
            case GameState::Quit:
            {
                return;
            }
       }

   }
}
