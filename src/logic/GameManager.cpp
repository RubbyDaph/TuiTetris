#include "GameManager.h"

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

std::string GameManager::GetFrame()  const
{
    return board.Render();
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
