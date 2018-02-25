#include <Utils/Keyboard.h>

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <exception>
#include <thread>
#include <atomic>
#include <vector>
#include <unordered_map>

namespace SnakeAI
{
    namespace Keyboard
    {
        static std::atomic<bool> processingKeyboardEvents = false;
        
        static HANDLE hStdin; 
        static DWORD fdwSaveOldMode;

        static std::thread pollingThread;
        static Handle nextAvailableHandle;
        static std::unordered_map<Handle, std::function<void(KeyEvent)>> handlers;

        void keyboardPollingFunction()
        {
            DWORD numberOfEvents;
            while (processingKeyboardEvents)
            {
                if (!GetNumberOfConsoleInputEvents(hStdin, &numberOfEvents))
                {
                    throw std::exception("Failed to get the number of events");
                }

                if (numberOfEvents == 0)
                {
                    Sleep(10);
                    continue;
                }
                
                std::vector<INPUT_RECORD> inputBuffer;
                inputBuffer.resize(numberOfEvents);

                DWORD numberOfEventsRead;
                if (!ReadConsoleInput(hStdin, inputBuffer.data(), numberOfEvents, &numberOfEventsRead)) 
                {
                    throw std::exception("Failed to read the events");
                }

                for (unsigned int i = 0; i < numberOfEventsRead; i++) 
                {
                    auto& event = inputBuffer[i];
                    if (event.EventType == KEY_EVENT && event.Event.KeyEvent.bKeyDown) 
                    { 
                        KeyEvent eventToFire;

                        const auto& keyCode = event.Event.KeyEvent.wVirtualKeyCode;

                        if ((keyCode >= '0' && keyCode <= '9') ||
                            (keyCode >= 'a' && keyCode <= 'z') ||
                            (keyCode >= 'A' && keyCode <= 'Z'))
                        {
                            eventToFire.isSpecial = false;
                            eventToFire.key.value = static_cast<char>(keyCode);
                        }
                        else
                        {   
                            eventToFire.isSpecial = true;
                            switch (keyCode)
                            {
                                case VK_UP:
                                    eventToFire.key.special = SpecialKey::ArrowUp;
                                    break;
                                case VK_DOWN:
                                    eventToFire.key.special = SpecialKey::ArrowDown;
                                    break;                                    
                                case VK_LEFT:
                                    eventToFire.key.special = SpecialKey::ArrowLeft;
                                    break;
                                case VK_RIGHT:
                                    eventToFire.key.special = SpecialKey::ArrowRight;
                                    break;
                                default:
                                    // unsupported key type
                                    continue;
                            }
                        }

                        for (auto& handler : handlers)
                        {
                            handler.second(eventToFire);
                        }
                    }
                }
            }
        }

        void startProcessingKeyboardEvents()
        {
            if (processingKeyboardEvents)
            {
                return;
            }

            hStdin = GetStdHandle(STD_INPUT_HANDLE);
            if (hStdin == INVALID_HANDLE_VALUE) 
            {
                throw std::exception("Failed to get the input handle");   
            }

            if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
            {
                throw std::exception("Failed to get the console mode");   
            } 

            // TODO: do we need the mouse input?
            DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
            if (!SetConsoleMode(hStdin, fdwMode))
            {
                throw std::exception("Failed to set the console mode");
            }

            processingKeyboardEvents = true;

            pollingThread = std::thread(keyboardPollingFunction);
        }

        void stopProcessingKeyboardEvents()
        {
            if (!processingKeyboardEvents)
            {
                return;
            }

            if (!SetConsoleMode(hStdin, fdwSaveOldMode))
            {
                throw std::exception("Failed to set the console mode");
            }

            processingKeyboardEvents = false;

            pollingThread.join();
        }

        Handle subscribeToEvents(std::function<void(KeyEvent)> handler)
        {
            handlers[++nextAvailableHandle] = handler;
            return nextAvailableHandle;
        }

        void unSubscribeHandle(Handle handle)
        {
            auto iHandler = handlers.find(handle);

            if (iHandler == handlers.end())
            {
                throw std::exception("Unexpected handle");
            }

            handlers.erase(iHandler);
        }
    }
}
#else
#error Not implemnted for other platforms
#endif