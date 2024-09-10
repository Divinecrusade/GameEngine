#pragma once

#include "Rectangle2D.hpp"
#include "IWindow.hpp"
#include "WinApiException.hpp"


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

        static Geometry::Rectangle2D<int> get_cursor_area()
        {
            CURSORINFO cursor_info{ };
            cursor_info.cbSize = sizeof(CURSORINFO);
            SIZE       cursor_size{ };
            
            if (HCURSOR const cursor_handler{ GetCursor() }; cursor_handler == NULL) throw std::runtime_error{ "Failed to get cursor info, because there is no cursor" };
            else if (!GetCursorInfo(&cursor_info)) throw WinApiException{ "Failed to get cursor info" };
            else if (ICONINFO icon_info{ }; !GetIconInfo((HICON)cursor_handler, &icon_info)) throw WinApiException{ "Failed to get icon info" };
            else if (BITMAP   icon_bitmap{ }; GetObjectW(icon_info.hbmMask, sizeof(icon_bitmap), &icon_bitmap) != sizeof(icon_bitmap)) throw WinApiException{ "Failed to get bitmap info" };
            else
            {
                cursor_size.cx = icon_bitmap.bmWidth;
                cursor_size.cy = icon_info.hbmColor ? icon_bitmap.bmHeight : icon_bitmap.bmHeight / 2;

                if (!DeleteObject(icon_info.hbmMask))  throw std::runtime_error{ "Failed to delete bmMask object" };
                if (icon_info.hbmColor != NULL && !DeleteObject(icon_info.hbmColor)) throw std::runtime_error{ "Failed to delete bmColor object" };
            }

            return Geometry::Rectangle2D<int>{ Geometry::Vector2D<int>{ cursor_info.ptScreenPos.x, cursor_info.ptScreenPos.y }, cursor_size.cx, cursor_size.cy };
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