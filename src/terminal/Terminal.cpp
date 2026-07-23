#include "Terminal.h"
#include <cerrno>
#include <unistd.h>
#include <system_error>
#include <stdexcept>


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
    if(isatty(STDIN_FILENO) != 1)
    {
        throw std::runtime_error("Standart input is not connected to a terminal");
    }

    if(isatty(STDOUT_FILENO) != 1)
    {
        throw std::runtime_error("Standart output is not connected to a terminal");
    }

    if(tcgetattr(STDIN_FILENO, &originalSettings) == -1) // getting original settings from terminal
    {
        throw std::system_error(errno, std::generic_category(), "Failed to read terminal settings");
    }

    termios gameSettings = originalSettings;

    gameSettings.c_lflag &= ~ICANON; // exiting canon mode, 
                                    //we can enter the symbols without enter
    gameSettings.c_lflag &= ~ECHO; // disable input visualization or smthing like that
    
    // these dudes controls how read() behave in non canon mode
    gameSettings.c_cc[VMIN] = 0;
    gameSettings.c_cc[VTIME] = 0;
    
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &gameSettings) == -1) // setting up our new settings
    {
        throw std::system_error(errno, std::generic_category(), "Failed to change terminal settings");
    }

    settingsChanged = true;
    
    std::string setup;
    
    setup += ALT_SCREEN; // go to alternative screen
    setup += HIDE_CURSOR; // hide cursor
    setup += CLEAR_ALL_SCREEN; // clear entire screen
    setup += MOVE_CURSOR_TO_00; // move cursor to 0 0 
    
    alternativeScreenEnabled = true;

    try
    {
        WriteAll(setup);
    }
    catch(...)
    {
        Restore();
        throw;
    }
}

Terminal::~Terminal() noexcept
{
    Restore();
}

void Terminal::Present(std::string_view frame)
{
    std::string output;
    output += MOVE_CURSOR_TO_00;
    output += CLEAR_FROM_CURSOR;
    output += frame;

    WriteAll(output);
}

void Terminal::WriteAll(std::string_view data)
{
    std::size_t totalWritten = 0;

    while(totalWritten < data.size())
    {
        const ssize_t written = write(STDOUT_FILENO, data.data() + totalWritten, data.size() - totalWritten);
        if(written > 0)
        {
            totalWritten += static_cast<std::size_t>(written);
            continue;
        }

        if(written == -1 && errno == EINTR)
        {
            continue;
        }

        if(written == 0)
        {
            throw std::runtime_error("Terminal write returned zero");
        }
        
        throw std::system_error(errno, std::generic_category(), "Failed to write to terminal");
    }
}

void Terminal::Restore() noexcept
{
    if(alternativeScreenEnabled)
    {

        std::string reset;
        reset += CLEAR_STYLE;
        reset += SHOW_CURSOR;
        reset += CLOSE_ALT_SCREEN;

       try
       {
           WriteAll(reset);
       }
       catch(...)
       {

       }

       alternativeScreenEnabled = false;
    }

    if(settingsChanged)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);

        settingsChanged = false;
    }
}
