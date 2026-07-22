#include "PausedMenu.h"

void PausedMenu::GoNextOption()
{
    if(option == MenuOption::Quit)
    {
        option = MenuOption::Resume;
    }
    else
    {
        option = static_cast<MenuOption>(static_cast<int>(option) + 1);
    }
}

void PausedMenu::GoPrevOption()
{
    if(option == MenuOption::Resume)
    {
        option = MenuOption::Quit;
    }
    else
    {
        option = static_cast<MenuOption>(static_cast<int>(option) - 1);
    }
}

MenuOption PausedMenu::ConfirmOption()
{
    return option;
}

std::string PausedMenu::Render() const
{
    std::string result;

    result += "┌────────────────────┐\n";
    for(int y = 0; y < height; y++)
    {
        result += "│";
    
        for(int x = 0; x < width; x++)
        {
            if(y == 2 && x == 2)
            {
                result += (option == MenuOption::Resume)? "> Resume" : "  Resume";
                x += 3;
                continue;
            }
            else if(y == 4 && x == 2)
            {
                result += (option == MenuOption::Restart)? "> Restart " : "  Restart ";
                x += 4;
                continue;
            }
            else if(y == 6 && x == 2)
            {
                result += (option == MenuOption::Quit)? "> Quit" : "  Quit";
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
