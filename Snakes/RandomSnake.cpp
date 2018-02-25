#include <Snakes/RandomSnake.h>

#include <chrono>
#include <random>
#include <vector>

using namespace std;

namespace SnakeAI
{
    Direction RandomSnake::makeMove(const State& state)
    {
        vector<Direction> directions {Direction::Up, Direction::Down, Direction::Left, Direction::Right};

        // TODO: seed should be configurable
        static mt19937 mt_rand(static_cast<unsigned int>(time(nullptr)));

        while (directions.size() > 0)
        {
            int index = mt_rand() % directions.size();

            if (state.isValidMove(directions[index]))
            {
                return directions[index];
            }
            
            directions.erase(directions.begin() + index);
        }

        // no valid moves
        return Direction::Up;
    }
}