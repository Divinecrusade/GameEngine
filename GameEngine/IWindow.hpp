#pragma once

#include "WinKey.hpp"

#include <optional>


namespace GameEngine::Interfaces
{
    __interface IWindow
    {
    public:

        virtual bool is_fun_key_pressed(WinKey key)   const = 0;
        virtual bool is_non_fun_key_pressed(int code) const = 0;
        virtual std::optional<WinKey> get_last_pressed_functional_key()     const = 0;
        virtual std::optional<int>    get_last_pressed_non_functional_key() const = 0;
        virtual void process_messages_queue()   = 0;
        virtual bool is_terminated()      const = 0;
        virtual HWND get_window_handler() const = 0;
    };
}