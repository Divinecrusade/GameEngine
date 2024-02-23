#pragma once

#include "Vector2D.hpp"
#include "IWindow.hpp"

#include <Windows.h>


namespace GameEngine
{
    class Mouse
    {
    public:

        __forceinline static Geometry::Vector2D<int> get_cursor_pos() noexcept
        {
            POINT const global_cursor_pos{ get_global_pos() };
            
            return { global_cursor_pos.x, global_cursor_pos.y };
        }
        __forceinline static Geometry::Vector2D<int> get_cursor_pos(Interfaces::IWindow const& wnd) noexcept
        {
            POINT cursor_pos{ get_global_pos() };

            ScreenToClient(wnd.get_window_handler(), &cursor_pos);

            return { cursor_pos.x, cursor_pos.y };
        }

    private:

        __forceinline static POINT get_global_pos() noexcept
        {
            POINT global_cursor_pos{ };

            GetCursorPos(&global_cursor_pos);

            return global_cursor_pos;
        }
    };
}