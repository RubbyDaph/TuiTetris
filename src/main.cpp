#include "logic/GameManager.h"
#include <iostream>
int main()
{
    GameManager manager;
    
    std::cout << manager.GetFrame();
    std::cout << manager.MakeStep();
    std::cout << manager.GetFrame();
    return 0;
}
