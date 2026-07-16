#include "GameManager.h"

GameManager::GameManager()
{
    
}

std::string GameManager::GetFrame()  const
{
    return board.Render();
}

std::string GameManager::MakeStep() 
{
    BoardStepResult boardStepResult = board.TryMoveDown();

    switch(boardStepResult.stepResult)
    {
        case StepResult::Moved: return "Yep, it moved\n";
        case StepResult::GameOver: return "Something is not good\n";
        case StepResult::Locked: return "even more questions\n";
    }
    return "WHAT";
}
