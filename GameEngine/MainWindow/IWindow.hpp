#pragma once

#include <Windows.h>
#include <WinUser.h>


namespace GameEngine2D
{
    namespace Interfaces
    {
        __interface IWindow
        {
        public:

            virtual void process_messages_queue() = 0;
            virtual bool is_terminated() const = 0;
            virtual HWND get_window_handler() const = 0;
        };
    }
}