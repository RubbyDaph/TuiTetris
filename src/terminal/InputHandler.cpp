#include "InputHandler.h"


bool InputHandler::WaitForInput(int timeoutMs)
{
    pollfd descriptor{};

    descriptor.fd = STDIN_FILENO;
    descriptor.events = POLLIN;

    const int result = ::poll(&descriptor, 1, timeoutMs);

    if(result < 0)
    {
        throw std::runtime_error("poll() failed");
    }
    return result > 0 && (descriptor.revents & POLLIN);
}

std::optional<Key> InputHandler::GetKey(int timeoutMs)
{
    if(!WaitForInput(timeoutMs)) return std::nullopt;

    char byte{};

    if(read(STDIN_FILENO, &byte, 1) != 1) return std::nullopt;

    if(byte == 'a' || byte == 'A') return Key::A;

    if(byte == 'd' || byte == 'D') return Key::D;

    if(byte == '\n' || byte == '\r') return Key::Enter;

    if(byte != '\x1b') return Key::Other;

    if(!WaitForInput(5)) return Key::Escape;
    
    char secondByte{};

    if(read(STDIN_FILENO, &secondByte, 1) != 1) return Key::Escape;

    if(secondByte != '[') return Key::Escape;

    if(!WaitForInput(5)) return Key::Escape;
    
    char thirdByte{};

    if(read(STDIN_FILENO, &thirdByte, 1) != 1) return Key::Escape;

    switch(thirdByte)
    {
        case 'A': return Key::ArrowUp;
        case 'B': return Key::ArrowDown;
        case 'C': return Key::ArrowRight;
        case 'D': return Key::ArrowLeft;
        default: return Key::Other;
    }
}
