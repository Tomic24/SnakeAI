#pragma once
#include <Engine/State.h>

namespace SnakeAI
{
    // Interface that the game display needs to implement
    class IDisplay
    {
    public:
        virtual void render(const State& state) = 0;
    };
}