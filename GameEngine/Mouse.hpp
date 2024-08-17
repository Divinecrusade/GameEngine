#pragma once

#include "Vector2D.hpp"
#include "IWindow.hpp"
#include "WinApiException.hpp"

#include <Windows.h>


namespace GameEngine
{
    class Mouse final
    {
    public:

        Mouse() = delete;
        Mouse(Mouse const&) = delete;
        Mouse(Mouse&&) = delete;

        Mouse& operator=(Mouse const&) = delete;
        Mouse& operator=(Mouse&&) = delete;

        ~Mouse() = delete;


        static Geometry::Vector2D<int> get_cursor_pos()
        {
            POINT const global_cursor_pos{ get_global_pos() };
            
            return { global_cursor_pos.x, global_cursor_pos.y };
        }

        static Geometry::Vector2D<int> get_cursor_pos(Interfaces::IWindow const& wnd)
        {
            POINT cursor_pos{ get_global_pos() };

            if (!ScreenToClient(wnd.get_window_handler(), &cursor_pos)) throw std::runtime_error{ "Failed to convert global mouse position to window local position" };

            return { cursor_pos.x, cursor_pos.y };
        }

    private:

        static POINT get_global_pos()
        {
            POINT global_cursor_pos{ };

            if (!GetCursorPos(&global_cursor_pos)) throw WinApiException{ "Failed to get cursor position" };

            return global_cursor_pos;
        }
    };
}