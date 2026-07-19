#include "logic/GameManager.h"
#include "terminal/Terminal.h"

#include <chrono>
#include <thread>

int main()
{
    Terminal terminal;
    GameManager manager;
    
    for(int i = 0; i < 23; i++)
    {
        terminal.Present(manager.GetFrame());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        manager.Tick();
    }

    return 0;
}
