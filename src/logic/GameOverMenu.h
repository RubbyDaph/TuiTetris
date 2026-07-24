#pragma once
#include <string>

enum class GameOverMenuOption 
{
    Restart,
    Quit
};

class GameOverMenu
{
public:
    GameOverMenu() = default;
    ~GameOverMenu() = default;

    std::string Render(unsigned int score) const;
    void GoNextOption();
    void GoPrevOption();
    GameOverMenuOption ConfirmOption(){return option;};
private:
    static constexpr int height = 9;
    static constexpr int width = 10;

    GameOverMenuOption option = GameOverMenuOption::Restart;
};
