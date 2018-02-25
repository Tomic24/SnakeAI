#pragma once

#include <functional>

namespace SnakeAI
{
    namespace Keyboard
    {
        typedef int Handle;

        enum class SpecialKey : uint8_t
        {
            ArrowUp,
            ArrowDown,
            ArrowLeft,
            ArrowRight
        };

        union Key
        {
            char value;
            SpecialKey special;
        };

        struct KeyEvent
        {
            Key key;
            bool isSpecial;  
        };

        void startProcessingKeyboardEvents();
        void stopProcessingKeyboardEvents();

        Handle subscribeToEvents(std::function<void(KeyEvent)> handler);
        void unSubscribeHandle(Handle handle);
    }
}