#include "GameManager.h"

GameManager::GameManager()
{
   board = new BoardClass(this->GenerateNextFigure());   
}

GameManager::~GameManager()
{
    delete board;
}

std::string GameManager::GetFrame()  const
{
    return board->Render();
}

FigureType GameManager::GenerateNextFigure() const
{
    std::random_device device;

    std::mt19937 gen(device());

    std::uniform_int_distribution<int> distrib(0, 6);

    return (FigureType)distrib(gen);
}

void GameManager::Tick() 
{
    if(gameState != GameState::Running) return;

    BoardStepResult boardStepResult = board->TryMoveDown();

    if(boardStepResult.stepResult == StepResult::Locked)
    {
        lineCount += boardStepResult.clearedLines;
        score += boardStepResult.clearedLines * (int)ScoreValue::ClearedLine;
        score += (int)ScoreValue::PlacedBlock;
        if(!board->Spawn(this->GenerateNextFigure()))
        {
            gameState = GameState::GameOver;
            std::cout << "WHAT WHAT WHAT\n";
            std::cout << board->getYep(); 
        }
    }
}
