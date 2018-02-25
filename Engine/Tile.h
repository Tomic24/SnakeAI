#pragma once

#include <vector>

namespace SnakeAI
{
    typedef int Tile;

    Tile emptyTile();

    bool isSnake(const Tile& t);

    bool isSnakeTail(const Tile& t);
    
    bool isEmpty(const Tile& t);

    bool isFood(const Tile& t);
}