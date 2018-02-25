#pragma once
#include <Engine/IPlayer.h>

namespace SnakeAI
{
    // Simple snake implementation that chooses a random valid direction to go in.
    class RandomSnake : public IPlayer
    {
    public:
        virtual Direction makeMove(const State& state);
    };
}