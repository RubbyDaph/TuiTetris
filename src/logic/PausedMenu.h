#include <string>

enum class MenuOption
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
    MenuOption ConfirmOption();
private:

    static constexpr int height = 10;
    static constexpr int width = 10;

    MenuOption option = MenuOption::Resume;

    
};


