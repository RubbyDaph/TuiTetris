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

    terminal.Present(manager.GetFrame());
    //        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    manager.Tick();
    }
    catch(const std::exception& error)
    {
        
    }
    return 0;
}
