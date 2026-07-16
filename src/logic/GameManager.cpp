#include "GameManager.h"

GameManager::GameManager()
{
    
}

std::string GameManager::GetFrame()  const
{
    return board.Render();
}
