#include "GameOverMenu.h"

void GameOverMenu::GoNextOption()
{
    if(option == GameOverMenuOption::Restart) option = GameOverMenuOption::Quit;
    else option = GameOverMenuOption::Restart;
}

void GameOverMenu::GoPrevOption()
{
    if(option == GameOverMenuOption::Quit) option = GameOverMenuOption::Restart;
    else option = GameOverMenuOption::Quit;
}

std::string GameOverMenu::Render() const
{
    std::string result;

    result += "┌────────────────────┐\n";
    for(int y = 0; y < height; y++)
    {
        result += "│";
    
        for(int x = 0; x < width; x++)
        {
            if(y == 1 && x == 3)
            {
                result += "GAME OVER ";
                x += 4;
                continue;
            }
            else if(y == 4 && x == 2)
            {
                result += (option == GameOverMenuOption::Restart)? "> Restart " : "  Restart ";
                x += 4;
                continue;
            }
            else if(y == 6 && x == 2)
            {
                result += (option == GameOverMenuOption::Quit)? "> Quit" : "  Quit";
                x += 2; 
                continue;
            }
            else 
            {
                result += "  ";
            }
        }
        result += "│\n";
    }
    
    result += "└────────────────────┘\n";
    return result;

}
