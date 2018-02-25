#pragma once
#include <Engine/State.h>

namespace SnakeAI
{
    // Interface that the game player needs to implement
    class IPlayer
    {
    public:
        // For a given state gived the direction the snake needs to move in
        virtual Direction makeMove(const State& state) = 0;
    };
}