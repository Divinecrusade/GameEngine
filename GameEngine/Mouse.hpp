#pragma once

#include "Vector2D.hpp"

#include <Windows.h>


namespace GameEngine
{
    class Mouse
    {
    public:

        __forceinline static Geometry::Vector2D<int> get_pos_on_screen() noexcept
        {
            POINT mouse_pos{ };
            
            GetCursorPos(&mouse_pos);

            return { mouse_pos.x, mouse_pos.y };
        }
    };
}