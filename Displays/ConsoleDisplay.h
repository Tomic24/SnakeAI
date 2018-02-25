#pragma once
#include <Engine/IDisplay.h>

#include <memory>

namespace SnakeAI
{
    // The platform-specific implementation of the console manipulation
    class ConsoleDisplayImpl;

    // Simple display that displays the game state in the console
    class ConsoleDisplay : public IDisplay
    {
    public:
        ConsoleDisplay(int width, int height);
        // The destructor is explicitly not inlined since
        // ConsoleDisplayImpl is forward declared and the
        // impl's destruction must happen where it's defined.
        ~ConsoleDisplay();
        virtual void render(const State& state);

    private:
        std::unique_ptr<ConsoleDisplayImpl> m_impl;
    };
}