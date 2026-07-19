#include "Terminal.h"
#include <cerrno>
#include <unistd.h>
#include <system_error>


#define ALT_SCREEN          "\x1b[?1049h" 
#define CLOSE_ALT_SCREEN    "\x1b[?1049l"
#define HIDE_CURSOR         "\x1b[?25l" 
#define SHOW_CURSOR         "\x1b[?25h"
#define CLEAR_ALL_SCREEN    "\x1b[2J"
#define CLEAR_FROM_CURSOR   "\x1b[J"
#define MOVE_CURSOR_TO_00   "\x1b[H"
#define CLEAR_STYLE         "\x1b[0m" 

Terminal::Terminal()
{
    isatty(STDIN_FILENO);

    tcgetattr(STDIN_FILENO, &originalSettings); // getting original settings from terminal

    termios gameSettings = originalSettings;

    gameSettings.c_lflag &= ~ICANON; // exiting canon mode, 
                                    //we can enter the symbols without enter
    gameSettings.c_lflag &= ~ECHO; // disable input visualization or smthing like that
    
    // these dudes controls how read() behave in non canon mode
    gameSettings.c_cc[VMIN] = 0;
    gameSettings.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &gameSettings); // setting up our new settings
    
    std::string setup;
    
    setup += ALT_SCREEN; // go to alternative screen
    setup += HIDE_CURSOR; // hide cursor
    setup += CLEAR_ALL_SCREEN; // clear entire screen
    setup += MOVE_CURSOR_TO_00; // move cursor to 0 0 

    WriteAll(setup);
}

Terminal::~Terminal() noexcept
{
    std::string reset;
    reset += CLEAR_STYLE; // clear colors and style
    reset += SHOW_CURSOR; // show cursor
    reset += CLOSE_ALT_SCREEN; // turn off alternative screen
    WriteAll(reset);

    // setting up original settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalSettings);
}

void Terminal::Present(std::string_view frame)
{
    std::string output;
    output += MOVE_CURSOR_TO_00;
    output += frame;
    output += CLEAR_FROM_CURSOR;

    WriteAll(output);
}

void Terminal::WriteAll(std::string_view data)
{
    std:;std::size_t totalWritten = 0;

    while(totalWritten < data.size())
    {
        const ssize_t written = write(STDOUT_FILENO, data.data() + totalWritten, data.size() - totalWritten);
        if(written > 0)
        {
            totalWritten += static_cast<std::size_t>(written);
            continue;
        }
        
        throw std::system_error(errno, std::generic_category(), "Failed to write to terminal");
    }
}
