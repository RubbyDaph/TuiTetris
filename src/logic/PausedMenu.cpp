#include "PausedMenu.h"

void PausedMenu::Reset()
{
    option = PausedMenuOption::Resume;
}

void PausedMenu::GoNextOption()
{
    if(option == PausedMenuOption::Quit)
    {
        option = PausedMenuOption::Resume;
    }
    else
    {
        option = static_cast<PausedMenuOption>(static_cast<int>(option) + 1);
    }
}

void PausedMenu::GoPrevOption()
{
    if(option == PausedMenuOption::Resume)
    {
        option = PausedMenuOption::Quit;
    }
    else
    {
        option = static_cast<PausedMenuOption>(static_cast<int>(option) - 1);
    }
}

PausedMenuOption PausedMenu::ConfirmOption()
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
            if(y == 1 && x == 3)
            {
                result += "PAUSED";
                x += 2;
                continue;
            }
            if(y == 4 && x == 2)
            {
                result += (option == PausedMenuOption::Resume)? "> Resume" : "  Resume";
                x += 3;
                continue;
            }
            else if(y == 6 && x == 2)
            {
                result += (option == PausedMenuOption::Restart)? "> Restart " : "  Restart ";
                x += 4;
                continue;
            }
            else if(y == 8 && x == 2)
            {
                result += (option == PausedMenuOption::Quit)? "> Quit" : "  Quit";
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
