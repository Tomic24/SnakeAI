#pragma once
#include <Engine/State.h>
#include <Engine/IPlayer.h>
#include <Engine/IDisplay.h>
#include <chrono>

namespace SnakeAI
{
    struct GameSettings
    {
        // Height of the board
        int m_height;
        // Width of th eboard
        int m_width;
        // Time between moves (for display purposes) - if the player
        // makes a move faster than this the game will wait until this interval passes
        std::chrono::milliseconds m_moveTime;
        // Maximum time between moves - in case the player doesn't make
        // a move in this timeframe the previous move will be taken for this turn as well.
        std::chrono::milliseconds m_maxMoveTime;
        // Maximum number of moves the game can take.
        int m_maxMoves;
    };

    struct GameResult
    {
        // Number of moves made in the game
        int m_moves;
        // Final length of the snake
        int m_snakeLength;
    };

    class Game
    {
    public:
        Game(const GameSettings& settings);
        GameResult Play(IPlayer& player, IDisplay& display);

    private:
        GameSettings m_settings;
    };
}