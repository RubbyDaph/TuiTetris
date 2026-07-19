#include "logic/GameManager.h"
#include "terminal/Terminal.h"
int main()
{
    Terminal terminal;
    GameManager manager;
    
    for(int i = 0; i < 23; i++)
    {
        terminal.Present(manager.GetFrame());
        manager.Tick();
    }

    return 0;
}
