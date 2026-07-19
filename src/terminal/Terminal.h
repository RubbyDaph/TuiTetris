#pragma once

#include <termios.h>

#include <string_view>

class Terminal
{
public:
    Terminal();
    ~Terminal() noexcept;

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;
    Terminal& operator=(Terminal&&) = delete;

    void Present(std::string_view frame);

private:
    void WriteAll(std::string_view data);
    void Restore() noexcept;

    bool settingsChanged = false;
    bool alternativeScreenEnabled = false;

    termios originalSettings{}; 
};
