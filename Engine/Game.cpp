#include <Engine/Game.h>

#include <chrono>
#include <condition_variable>
#include <random>
#include <thread>
#include <atomic>

using namespace std;

namespace SnakeAI
{
    bool areGameSettingsValid(const GameSettings& settings)
    {
        const int maxDimension = 1000;

        return settings.m_height > 1 && settings.m_height < maxDimension &&
            settings.m_width > 1 && settings.m_width < maxDimension &&
            settings.m_maxMoves > 0 && settings.m_maxMoves < 10000000 &&
            settings.m_moveTime <= settings.m_maxMoveTime &&
            settings.m_maxMoveTime < std::chrono::seconds(5);
    }

    Game::Game(const GameSettings& settings) : m_settings(settings)
    {
        if (!areGameSettingsValid(m_settings))
        {
            throw std::exception("Game settings not valid");
        }
    }

    Position generateFoodPosition(const State& state, mt19937& random)
    {
        const Tile* board = state.getBoardData();
        const int boardSize = state.getBoardWidth() * state.getBoardHeight();

        int generatedIndex = random() % (boardSize - state.getSnakeSize());

        for (int i = 0; i < boardSize; i++)
        {
            if (isEmpty(board[i]))
            {
                if (generatedIndex == 0)
                {
                    Position result;
                    result.m_x = i % state.getBoardWidth();
                    result.m_y = i / state.getBoardWidth();
                    return result;
                }

                generatedIndex--;
            }
        }

        throw std::exception("Unexpected board data");
    }

    GameResult Game::Play(IPlayer& player, IDisplay& display)
    {
        // Use a mersenne twister as a random number generator
        // TODO: seed should be configurable
        mt19937 mt_rand(static_cast<unsigned int>(time(nullptr)));
        
        Position headPosition;
        headPosition.m_x = mt_rand() % m_settings.m_width;
        headPosition.m_y = mt_rand() % m_settings.m_height;

        State state(m_settings.m_width, m_settings.m_height, headPosition);
        state.setFood(generateFoodPosition(state, mt_rand));

        bool gameOver = false;

        // For synchronization puposes, the game and player will have
        // individual iteration counters
        int gameIteration = 0;
        int playerIteration = 0;
        // A condition variable & mutex will be used for syncrhonization
        condition_variable cv;
        mutex mtx;

        atomic<Direction> playerMove;

        std::thread playerThread([&]()
        {
            while (true)
            {
                // We need to wait until there is another iteration or the game gets finished
                unique_lock<mutex> lk(mtx);
                cv.wait(lk, [&] { return gameIteration > playerIteration || gameOver; });
                int currentIteration = gameIteration;
                lk.unlock();

                if (gameOver)
                {
                    break;
                }

                playerMove = player.makeMove(state);

                {
                    lock_guard<mutex> lk(mtx);
                    playerIteration = currentIteration;
                }

            }
        });

        while (true)
        {
            // Get the current timestamp
            auto iteration_start = chrono::high_resolution_clock::now();

            // Notify the player thread to make a move
            {
                lock_guard<mutex> lk(mtx);
                gameIteration++;
            }
            cv.notify_one();

            // Render the current state
            display.render(state);

            auto turnEnd = chrono::high_resolution_clock::now() + m_settings.m_maxMoveTime;
            
            // wait until the player made a move
            unique_lock<mutex> lk(mtx);
            cv.wait_until(lk, turnEnd, [&]{ return gameIteration == playerIteration; });

            if (!state.isValidMove(playerMove))
            {
                gameOver = true;
                break;
            }

            int snakeSize = state.getSnakeSize();
            state.update(playerMove);

            if (snakeSize < state.getSnakeSize() &&
                state.getSnakeSize() < state.getBoardHeight() * state.getBoardWidth())
            {
                state.setFood(generateFoodPosition(state, mt_rand));
            }

            // if more time remains wait until the next turn should start
            if (chrono::high_resolution_clock::now() < iteration_start + m_settings.m_moveTime)
            {
                auto sleepTime = iteration_start + m_settings.m_moveTime - chrono::high_resolution_clock::now(); 
                std::this_thread::sleep_for(sleepTime);
            }
        }

        cv.notify_one();
        playerThread.join();

        GameResult result;

        result.m_moves = gameIteration;
        result.m_snakeLength = state.getSnakeSize();

        return result;
    }
}