#include "GameOverMenu.h"

void GameOverMenu::Reset()
{
    option = GameOverMenuOption::Restart;
}

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

std::string GameOverMenu::Render(unsigned int score) const
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
            if(y == 3)
            {
                const std::string scoreText = std::to_string(score);
                const std::size_t leftPadding = 6;
                const std::size_t rightPadding = 20 - scoreText.size() - leftPadding - 6;
                result += std::string(leftPadding, ' ') + "Score:"  + scoreText + std::string(rightPadding, ' ');
                break;
            }
            else if(y == 5 && x == 2)
            {
                result += (option == GameOverMenuOption::Restart)? "> Restart " : "  Restart ";
                x += 4;
                continue;
            }
            else if(y == 7 && x == 2)
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
