#pragma once

#include "Vector2D.hpp"
#include <cassert>


namespace GameEngine::Geometry
{
    template<typename T>
    requires (std::is_arithmetic<T>() == true)
    struct Rectangle2D final
    {
        [[maybe_unused]] constexpr Rectangle2D() noexcept(noexcept(T{ })) = default;
        [[maybe_unused]] constexpr Rectangle2D(Rectangle2D const&) noexcept(noexcept(T(std::declval<T const&>()))) = default;
        [[maybe_unused]] constexpr Rectangle2D(T const& left, T const& right, T const& bottom, T const& top) noexcept(noexcept(T(std::declval<T const&>())))
        :
        left{ left },
        right{ right },
        bottom{ bottom },
        top{ top }
        { }
        [[maybe_unused]] constexpr Rectangle2D(Vector2D<T> const& top_left, Vector2D<T> const& bot_right) noexcept(noexcept(T(std::declval<T const&>())))
        :
        left{ top_left.x },
        right{ bot_right.x },
        bottom{ bot_right.y },
        top{ top_left.y }
        { }
        [[maybe_unused]] constexpr Rectangle2D(Vector2D<T> const& top_left, T const& width, T const& height) noexcept(noexcept(T(std::declval<T const&>())))
        :
        Rectangle2D{ top_left, Vector2D<T>{ top_left.x + width, top_left.y + height }}
        { }
        [[maybe_unused]] static Rectangle2D get_from_center(Vector2D<T> const& center, T const& width_half, T const& height_half) noexcept(noexcept(T(std::declval<T const&>())))
        {
            return Rectangle2D{ center - Vector2D<T>{ width_half, height_half }, center + Vector2D<T>{ width_half, height_half } };
        }
        [[maybe_unused]] constexpr Rectangle2D(Rectangle2D&&) noexcept = default;

        [[maybe_unused]] constexpr Rectangle2D& operator=(Rectangle2D const&) noexcept(noexcept(std::declval<T&>() = std::declval<T const&>())) = default;
        [[maybe_unused]] Rectangle2D& operator=(Rectangle2D&&) noexcept = default;

        ~Rectangle2D() noexcept = default;

        template<typename F>
        [[maybe_unused]] constexpr Rectangle2D<T>(Rectangle2D<F> const& rect) noexcept
        :
        left{ static_cast<T>(rect.left) },
        right{ static_cast<T>(rect.right) },
        bottom{ static_cast<T>(rect.bottom) },
        top{ static_cast<T>(rect.top) }
        { }

        template<typename F>
        [[maybe_unused]] constexpr Rectangle2D<T>& operator=(Rectangle2D<F> const& rect) noexcept
        {
            left = static_cast<T>(rect.left);
            right = static_cast<T>(rect.right);
            bottom = static_cast<T>(rect.bottom);
            top = static_cast<T>(rect.top);
            return *this;
        }

        [[maybe_unused]] constexpr T get_width() const noexcept(noexcept(std::declval<T const&>() - std::declval<T const&>()))
        {
            return right - left;
        }
        [[maybe_unused]] constexpr T get_height() const noexcept(noexcept(std::declval<T const&>() - std::declval<T const&>()))
        {
            return bottom - top;
        }
        [[maybe_unused]] constexpr Vector2D<T> get_center() const noexcept(noexcept(Vector2D<T>(std::declval<T const&>(), std::declval<T const&>())))
        {
            return Vector2D<T>{ (left + right) / static_cast<T>(2), (top + bottom) / static_cast<T>(2) };
        }
        [[maybe_unused]] constexpr Rectangle2D get_expanded(T offset) const noexcept(noexcept(std::declval<T const&>() - std::declval<T const&>()) && noexcept(std::declval<T const&>() + std::declval<T const&>()) && noexcept(T(std::declval<T const&>())))
        {
            return Rectangle2D(left - offset, right + offset, bottom + offset, top - offset);
        }

        [[maybe_unused]] constexpr bool is_colided_with(Rectangle2D const& rect) const noexcept(noexcept(std::declval<T const&>() > std::declval<T const&>()) && noexcept(std::declval<T const&>() < std::declval<T const&>()))
        {
            return left < rect.right &&
                    right > rect.left &&
                    bottom > rect.top &&
                    top < rect.bottom;
        }
        [[maybe_unused]] constexpr bool contains(Vector2D<T> const& point) const noexcept(noexcept(std::declval<T const&>() >= std::declval<T const&>()) && noexcept(std::declval<T const&>() <= std::declval<T const&>()))
        {
            return left <= point.x && right >= point.x &&
                    bottom >= point.y && top <= point.y;
        }
        [[maybe_unused]] constexpr bool contains(Rectangle2D const& rect) const noexcept(noexcept(std::declval<T const&>() >= std::declval<T const&>()) && noexcept(std::declval<T const&>() <= std::declval<T const&>()))
        {
            return left <= rect.left && right >= rect.right && bottom >= rect.bottom && top <= rect.top;
        }
        [[maybe_unused]] bool is_contained(Rectangle2D const& rect) const noexcept(noexcept(std::declval<T const&>() > std::declval<T const&>()) && noexcept(std::declval<T const&>() < std::declval<T const&>()))
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
    constexpr bool operator==(Rectangle2D<T> const& lhs, Rectangle2D<T> const& rhs) noexcept(noexcept(std::declval<T const&>() == std::declval<T const&>()))
    {
        return lhs.left == rhs.left && lhs.right == rhs.right && 
                lhs.bottom == rhs.bottom && lhs.top == rhs.top;
    }
    
    template<typename T>
    requires (std::is_arithmetic<T>() == true)
    constexpr bool operator!=(Rectangle2D<T> const& lhs, Rectangle2D<T> const& rhs) noexcept(noexcept(std::declval<T const&>() == std::declval<T const&>()))
    {
        return !(lhs == rhs);
    }
}