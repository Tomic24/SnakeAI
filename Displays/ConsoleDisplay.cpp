#include <Displays/ConsoleDisplay.h>

#ifdef _WIN32

#include <vector>

#include <windows.h>

using namespace std;

namespace SnakeAI
{
    class ConsoleDisplayImpl
    {
    public:
        void renderBuffer()
        {
            if (!WriteConsoleOutput(
                m_boardScreenBuffer,    // screen buffer to write to 
                m_boardBuffer.data(),   // buffer to copy from 
                m_boardSize,            // size of buffer 
                m_boardOffset,          // top left src cell in buffer 
                &m_consoleLocation))    // dest. screen buffer rectangle
            {
                throw std::exception("failed to write buffer");   
            }
        }

        ConsoleDisplayImpl(int width, int height) :
            m_boardSize {static_cast<SHORT>(width + 2), static_cast<SHORT>(height + 2)}, // +1 on each side for borders
            m_boardOffset {0, 0}, // no reason to have an offset bigger than this
            m_consoleLocation {static_cast<SHORT>(0), static_cast<SHORT>(0), static_cast<SHORT>(width + 1), static_cast<SHORT>(height + 1)}
        {
            m_boardBuffer.resize(m_boardSize.X * m_boardSize.Y);

            m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
            m_boardScreenBuffer = CreateConsoleScreenBuffer( 
                GENERIC_READ |           // read/write access 
                GENERIC_WRITE, 
                FILE_SHARE_READ | 
                FILE_SHARE_WRITE,        // shared 
                NULL,                    // default security attributes 
                CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
                NULL);                   // reserved; must be NULL 

            if (m_hStdout == INVALID_HANDLE_VALUE || m_boardScreenBuffer == INVALID_HANDLE_VALUE) 
            {
                throw std::exception("CreateConsoleScreenBuffer failed");
            }

            // Make the new screen buffer the active screen buffer. 
            if (!SetConsoleActiveScreenBuffer(m_boardScreenBuffer)) 
            {
                throw std::exception("SetConsoleActiveScreenBuffer failed"); 
            }

            // Hide the cursor
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(m_boardScreenBuffer, &cursorInfo);
            cursorInfo.bVisible = false; // set the cursor visibility
            SetConsoleCursorInfo(m_boardScreenBuffer, &cursorInfo);

            for (int x = 0; x < m_boardSize.X; x++)
            {
                for (int y = 0; y < m_boardSize.Y; y++)
                {
                    int index = x + y * m_boardSize.X;
                    // borders are white
                    if (x == 0 || y == 0 || x == m_boardSize.X - 1 || y == m_boardSize.Y - 1)
                    {
                        m_boardBuffer[index].Attributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
                        m_boardBuffer[index].Char.AsciiChar = ' ';
                    }
                    else
                    {
                        m_boardBuffer[index].Attributes = 0;
                        m_boardBuffer[index].Char.AsciiChar = ' ';
                    }
                }
            }        

            renderBuffer();    
        }

        ~ConsoleDisplayImpl()
        {
            SetConsoleActiveScreenBuffer(m_hStdout);
        }

        void render(const State& state)
        {
            if (state.getBoardHeight() + 2 != m_boardSize.Y ||
                state.getBoardWidth() + 2 != m_boardSize.X)
            {
                throw std::exception("Unexpected board size");
            }

            for (int x = 0; x < state.getBoardWidth(); x++)
            {
                for (int y = 0; y < state.getBoardHeight(); y++)
                {
                    Tile source = state.getTile(x, y);
                    int destIndex = (y + 1) * m_boardSize.X + x + 1;

                    if (isEmpty(source))
                    {
                        m_boardBuffer[destIndex].Char.AsciiChar = ' ';
                    }
                    else if (isFood(source))
                    {
                        m_boardBuffer[destIndex].Char.AsciiChar = -107;
                        m_boardBuffer[destIndex].Attributes = FOREGROUND_RED;
                    }
                    else if (isSnake(source))
                    {
                        if (source == state.getSnakeSize())
                        {
                            m_boardBuffer[destIndex].Char.UnicodeChar = 9;
                        }
                        else
                        {
                            m_boardBuffer[destIndex].Char.AsciiChar = 7;
                        }

                        m_boardBuffer[destIndex].Attributes = FOREGROUND_GREEN;
                    }
                }
            }

            renderBuffer();
        }

    private:
        HANDLE m_hStdout;
        HANDLE m_boardScreenBuffer;
        const COORD m_boardSize;
        const COORD m_boardOffset;
        SMALL_RECT m_consoleLocation;
        vector<CHAR_INFO> m_boardBuffer;
    };
}

#else
#error not implemented for other platforms
#endif

namespace SnakeAI
{
    ConsoleDisplay::ConsoleDisplay(int width, int height)
    {
        m_impl = make_unique<ConsoleDisplayImpl>(width, height);
    }

    ConsoleDisplay::~ConsoleDisplay() {}

    void ConsoleDisplay::render(const State& state)
    {
        m_impl->render(state);
    }
}