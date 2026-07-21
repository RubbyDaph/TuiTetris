#pragma once

#include <optional>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>

enum class Key
{
    ArrowLeft,
    ArrowRight,
    ArrowDown,
    A,
    D,
    Escape,
    P,
    Q,
    Other
};

class InputHandler
{
public:
    InputHandler() = default;
    ~InputHandler() = default;

    std::optional<Key> GetKey(int timeoutMs);
private:
    bool WaitForInput(int timeoutMs);
};
