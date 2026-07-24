#include "GameOverMenu.h"
#include <iomanip>
#include <sstream>

namespace
{
    std::string FormatScore(unsigned int score)
    {
        if(score < 100000)
        {
            return std::to_string(score);
        }

        const bool useMillions = score >= 1000000;
        const double divisor = useMillions ? 1000000.0 : 1000.0;

        std::ostringstream formatted;
        formatted << std::fixed << std::setprecision(1)
                  << static_cast<double>(score) / divisor
                  << (useMillions ? "M" : "K");

        return formatted.str();
    }
}

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
                const std::string scoreText = "Score: " + FormatScore(score);
                const std::size_t leftPadding = (20 - scoreText.size()) / 2;
                const std::size_t rightPadding =
                    20 - scoreText.size() - leftPadding;

                result += std::string(leftPadding, ' ')
                       + scoreText
                       + std::string(rightPadding, ' ');
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
