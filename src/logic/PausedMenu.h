#pragma once
#include <string>

enum class PausedMenuOption
{
    Resume,
    Restart,
    Quit
};

class PausedMenu
{
public:
    PausedMenu() = default;
    ~PausedMenu() = default;

    std::string Render() const;
    void GoNextOption();
    void GoPrevOption();
    PausedMenuOption ConfirmOption();
    void Reset();
private:

    static constexpr int height = 10;
    static constexpr int width = 10;

    PausedMenuOption option = PausedMenuOption::Resume;

    
};


