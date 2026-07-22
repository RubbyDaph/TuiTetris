#include "logic/GameManager.h"
#include "terminal/Terminal.h"
#include "terminal/InputHandler.h"


int main()
{
    
    try
    {
    Terminal terminal;
    InputHandler input;
    GameManager manager;

    manager.Run(terminal, input);
    }
    catch(const std::exception& error)
    {
        
    }
    return 0;
}
