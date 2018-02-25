#include <Engine/State.h>

#include <exception>

namespace SnakeAI
{
    void updatePosition(Position& position, Direction direction)
    {
        switch(direction)
        {
            case Direction::Up:
                position.m_x--;
                break;
            case Direction::Down:
                position.m_x++;
                break;
            case Direction::Left:
                position.m_y--;
                break;
            case Direction::Right:
                position.m_y++;
                break;
            default:
                throw std::exception("Unknown position");
        }
    }

    State::State(int width, int height, Position head)
    : m_head(head), m_width(width), m_height(height)
    {
        const int maxDimension = 1000;

        if (width <= 0 || height <= 0 || width >= maxDimension || height >= maxDimension)
        {
            throw std::exception("board dimensions");
        }

        if (!isPositionOnBoard(m_head))
        {
            throw std::exception("Invalid head position");
        }

        m_board.resize(width * height, emptyTile());
        setTile(m_head, 1);
    }

    bool State::isValidMove(Direction move) const
    {
        Position newHead(m_head);
        updatePosition(newHead, move);
        if (newHead.m_x < 0 || newHead.m_x >= m_width ||
            newHead.m_y < 0 || newHead.m_y >= m_height)
        {
            return false;
        }

        const Tile& newHeadTile = getTile(newHead); 
        return isPositionOnBoard(newHead) && (isEmpty(newHeadTile) || isFood(newHeadTile) || isSnakeTail(newHeadTile));
    }

    void State::update(Direction move)
    {
        if (!isValidMove(move))
        {
            throw std::exception("Invalid move");
        }

        Position newHeadPos(m_head);
        updatePosition(newHeadPos, move);

        if (isFood(getTile(newHeadPos)))
        {
            setTile(newHeadPos, ++m_snakeSize);
        }
        else
        {
            for (auto& tile : m_board)
            {
                if (tile > 0)
                {
                    tile--;
                }
            }
            setTile(newHeadPos, m_snakeSize);
        }

        m_head = newHeadPos;
    }

    void State::setFood(const Position& position)
    {
        if (!isEmpty(getTile(position)))
        {
            throw std::exception("Can't set food on occupied square");
        }

        setTile(position, -1);
    }
    
    void State::setTile(const Position& position, const Tile& tile)
    {
        if (!isPositionOnBoard(position))
        {
            throw std::exception("Invalid position");
        }

        m_board[getPositionIndex(position)] = tile;
    }

    bool State::isPositionOnBoard(const Position& position) const
    {
        return isPositionOnBoard(position.m_x, position.m_y);
    }

    bool State::isPositionOnBoard(int x, int y) const
    {
        return x < m_width && y < m_height && x >= 0 && y >= 0;
    }

    const Tile* State::getBoardData() const
    {
        return m_board.data();
    }
    
    int State::getPositionIndex(const Position& position) const
    {
        return getPositionIndex(position.m_x, position.m_y);
    }

    int State::getPositionIndex(int x, int y) const
    {
        return x + y * m_width;
    }

    const Tile& State::getTile(int x, int y) const
    {
        if (!isPositionOnBoard(x, y))
        {
            throw std::exception("Invalid position");
        }

        return m_board[getPositionIndex(x, y)];
    }

    const Tile& State::getTile(const Position& position) const
    {
        return getTile(position.m_x, position.m_y);
    }
}