#pragma once

#include "Vector2D.hpp"
#include <cassert>


namespace GameEngine
{
    namespace Geometry
    {
        template<typename T>
        requires (std::is_arithmetic<T>() == true)
        struct Rectangle2D final
        {
            Rectangle2D() = default;
            Rectangle2D(Rectangle2D const&) = default;
            Rectangle2D(T const& left, T const& right, T const& bottom, T const& top)
            :
            left{ left },
            right{ right },
            bottom{ bottom },
            top{ top }
            { 
                assert(left < right);
                assert(top < bottom);
            }
            Rectangle2D(Vector2D<T> const& top_left, Vector2D<T> const& bot_right)
            :
            left{ top_left.x },
            right{ bot_right.x },
            bottom{ bot_right.y },
            top{ top_left.y }
            { 
                assert(left < right);
                assert(top < bottom);
            }
            Rectangle2D(Vector2D<T> const& top_left, T const& width, T const& height)
            :
            Rectangle2D{ top_left, Vector2D<T>{ top_left.x + width, top_left.y + height }}
            { }
            static Rectangle2D get_from_center(Vector2D<T> const& center, T const& width_half, T const& height_half)
            {
                return Rectangle2D{ center - Vector2D<T>{ width_half, height_half }, center + Vector2D<T>{ width_half, height_half } };
            }
            Rectangle2D(Rectangle2D&&) = default;

            Rectangle2D& operator=(Rectangle2D const&) = default;
            Rectangle2D& operator=(Rectangle2D&&) = default;

            ~Rectangle2D() = default;

            template<typename F>
            Rectangle2D<T>(Rectangle2D<F> const& rect)
            :
            left{ static_cast<T>(rect.left) },
            right{ static_cast<T>(rect.right) },
            bottom{ static_cast<T>(rect.bottom) },
            top{ static_cast<T>(rect.top) }
            { }

            template<typename F>
            Rectangle2D<T>&operator=(Rectangle2D<F> const& rect)
            {
                left = static_cast<T>(rect.left);
                right = static_cast<T>(rect.right);
                bottom = static_cast<T>(rect.bottom);
                top = static_cast<T>(rect.top);
                return *this;
            }

            T get_width() const
            {
                assert(left < right);

                return left - right;
            }
            T get_height() const
            {
                assert(top < bottom);

                return bottom - top;
            }
            Vector2D<T> get_center() const
            {
                return Vector2D<T>{ (left + right) / static_cast<T>(2), (top + bottom) / static_cast<T>(2) };
            }
            Rectangle2D get_expanded(T offset) const
            {
                return Rectangle2D(left - offset, right + offset, bottom + offset, top - offset);
            }

            bool is_colided_with(Rectangle2D const& rect) const
            {
                return left < rect.right &&
                       right > rect.left &&
                       bottom > rect.top &&
                       top < rect.bottom;
            }
            bool contains(Vector2D<T> const& point) const
            {
                return left <= point.x && right >= point.x &&
                       bottom >= point.y && top <= point.y;
            }
            bool contains(Rectangle2D const& rect) const
            {
                return left <= rect.left && right >= rect.right && bottom >= rect.bottom && top <= rect.top;
            }
            bool is_contained(Rectangle2D const& rect) const
            {
                return rect.contains(*this);
            }


            T left;
            T right;
            T bottom;
            T top;
        };
    
        template<typename T>
        requires (std::is_arithmetic<T>() == true)
        bool operator==(Rectangle2D<T> const& lhs, Rectangle2D<T> const& rhs) noexcept
        {
            return lhs.left == rhs.left && lhs.right == rhs.right && 
                   lhs.bottom == rhs.bottom && lhs.top == rhs.top;
        }
    
        template<typename T>
        requires (std::is_arithmetic<T>() == true)
        bool operator!=(Rectangle2D<T> const& lhs, Rectangle2D<T> const& rhs)
        {
            return !(lhs == rhs);
        }
    }
}