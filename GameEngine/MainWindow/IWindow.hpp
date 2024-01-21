#pragma once

#include <Windows.h>
#include "WinKey.hpp"


namespace GameEngine2D
{
    namespace Interfaces
    {
        __interface IWindow
        {
        public:

            virtual WinKey get_pressed_functional_key() const = 0;
            virtual int get_pressed_non_functional_key() const = 0;
            virtual void process_messages_queue() = 0;
            virtual bool is_terminated() const = 0;
            virtual HWND get_window_handler() const = 0;
        };
    }
}