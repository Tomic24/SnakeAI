#include <Engine/Tile.h>

namespace SnakeAI
{
    Tile emptyTile()
    {
        return 0;
    }

    bool isSnake(const Tile& t)
    {
        return t > 0;
    }

    bool isSnakeTail(const Tile& t)
    {
        return t == 1;
    }
    
    bool isEmpty(const Tile& t)
    {
        return t == 0;
    }

    bool isFood(const Tile& t)
    {
        return t < 0;
    }
}