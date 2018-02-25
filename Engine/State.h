#pragma once

#include <vector>

#include <Engine/Tile.h>

namespace SnakeAI
{
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    struct Position
    {
        int m_x;
        int m_y;
    };

    class State
    {
    public:
        State(int width, int height, Position head);

        bool isValidMove(Direction move) const;
        void update(Direction move);
        void setFood(const Position& position);

        const Tile* getBoardData() const;

        const Tile& getTile(int x, int y) const;
        const Tile& getTile(const Position& position) const;

        int getSnakeSize() const { return m_snakeSize; }
        int getBoardWidth() const { return m_width; }
        int getBoardHeight() const { return m_height; }

    private:
        void setTile(const Position& position, const Tile& tile);

        bool isPositionOnBoard(const Position& position) const;
        bool isPositionOnBoard(int x, int y) const;

        int getPositionIndex(const Position& position) const;
        int getPositionIndex(int x, int y) const;

        bool isHeadValid();

        std::vector<Tile> m_board;
        int m_width;
        int m_height;

        int m_snakeSize = 1;
        Position m_head;
    };
}