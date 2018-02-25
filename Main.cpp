#include <Engine/Game.h>
#include <Snakes/RandomSnake.h>
#include <Displays/ConsoleDisplay.h>

#include <chrono>
#include <iostream>

using namespace SnakeAI;
using namespace std;

GameResult playGameOfSnake()
{
    const int boardWidth = 3;
    const int boardHeight = 3;
    ConsoleDisplay display(boardWidth, boardHeight);
    GameSettings settings;
    settings.m_height = boardHeight;
    settings.m_width = boardWidth;
    settings.m_moveTime = chrono::milliseconds(300);
    settings.m_maxMoveTime = chrono::milliseconds(300);
    settings.m_maxMoves = 1000000;

    RandomSnake snake;

    Game game(settings);

    return game.Play(snake, display);
}

int main()
{
    GameResult result = playGameOfSnake();

    cout << "Game Finished!\nIterations played: " << result.m_moves << "\nSnake length: " << result.m_snakeLength << "\n";

    return 0;
}