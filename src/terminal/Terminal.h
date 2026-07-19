#pragma once

#include <termios.h>

#include <string_view>

class Terminal
{
public:
    Terminal();
    ~Terminal() noexcept;

    void Present(std::string_view frame);
private:
    void WriteAll(std::string_view data);

    termios originalSettings{}; 
};
