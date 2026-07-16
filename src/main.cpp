#include "logic/GameManager.h"
#include <iostream>
int main()
{
    GameManager manager;
    
    for(int i = 0; i < 23; i++)
    {
        std::cout << manager.GetFrame();
        manager.Tick();
    }

    return 0;
}
