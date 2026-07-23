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
private:

    static constexpr int height = 10;
    static constexpr int width = 10;

    PausedMenuOption option = PausedMenuOption::Resume;

    
};


