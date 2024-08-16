#pragma once

#include "Auxiliry.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>
#include <cmath>
#include <cassert>


namespace GameEngine::Geometry
{ 
    template<typename T>
    requires (std::is_arithmetic<T>() == true)
    struct Vector2D final
    {
        [[maybe_unused]] constexpr Vector2D() noexcept(noexcept(T{ })) = default;
        [[maybe_unused]] constexpr Vector2D(T const& x, T const& y) noexcept(noexcept(T{ x }) && noexcept(T{ y }))
        :
        x{ x },
        y{ y }
        { }
        [[maybe_unused]] constexpr Vector2D(Vector2D const&) noexcept(noexcept(T(std::declval<T const&>()))) = default;
        [[maybe_unused]] Vector2D(Vector2D&&) noexcept = default;
        [[maybe_unused]] constexpr Vector2D& operator=(Vector2D const&) noexcept(noexcept(std::declval<T&>() = std::declval<T const&>())) = default;
        [[maybe_unused]] Vector2D& operator=(Vector2D&&) noexcept = default;

        ~Vector2D() noexcept = default;

        template<typename F>
        [[maybe_unused]] constexpr Vector2D<T>(Vector2D<F> const& vec) noexcept
        :
        x{ static_cast<T>(vec.x) },
        y{ static_cast<T>(vec.y) }
        { }

        template<typename F>
        [[maybe_unused]] constexpr Vector2D<T>& operator=(Vector2D<F> const& vec) noexcept
        {
            x = static_cast<T>(vec.x);
            y = static_cast<T>(vec.y);
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator-=(Vector2D const& rhs) noexcept(noexcept(std::declval<T&>() -= std::declval<T const&>()))
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator+=(Vector2D const& rhs) noexcept(noexcept(std::declval<T&>() += std::declval<T const&>()))
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator/=(Vector2D const& rhs) noexcept(noexcept(std::declval<T&>() /= std::declval<T const&>()))
        {
            assert(rhs.x != static_cast<T>(0));
            assert(rhs.y != static_cast<T>(0));

            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator/=(T const& rhs) noexcept(noexcept(std::declval<T&>() /= std::declval<T const&>()))
        {
            assert(rhs != static_cast<T>(0));

            x /= rhs;
            y /= rhs;
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator*=(Vector2D const& rhs) noexcept(noexcept(std::declval<T&>() *= std::declval<T const&>()))
        {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        [[maybe_unused]] constexpr Vector2D& operator*=(T const& rhs) noexcept(noexcept(std::declval<T&>() *= std::declval<T const&>()))
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        [[maybe_unused]] static T get_square_length(Vector2D const& vec) noexcept(noexcept(std::declval<T const&>() * std::declval<T const&>()) && noexcept(std::declval<T const&>() + std::declval<T const&>()))
        {
            return vec.x * vec.x + vec.y * vec.y;
        }
            
        [[maybe_unused]] T get_square_length() const noexcept(noexcept(Vector2D<T>::get_square_length(std::declval<Vector2D<T> const&>())))
        {
            return Vector2D<T>::get_square_length(*this);
        }

        [[maybe_unused]] static double get_length(Vector2D const& vec)
        {
            return std::sqrt(get_square_length(vec));
        }

        [[maybe_unused]] double get_length() const
        {
            return Vector2D::get_length(*this);
        }

        [[maybe_unused]] static Vector2D get_normalized(Vector2D const& vec)
        {
            double const len{ get_length(vec) };
            if (!Auxiliry::is_equal_with_precision(len, 0.))
            {
                return vec / static_cast<T>(len);
            }
            return vec;
        }

        [[maybe_unused]] Vector2D get_normalized() const
        {
            return Vector2D<T>::get_normalized(*this);
        }

        [[maybe_unused]] Vector2D& normalize()
        {
            return *this = get_normalized(*this);
        }

        [[maybe_unused]] static Vector2D get_rotated(Vector2D const& vec, double radians)
        {
            Vector2D const tmp{ static_cast<T>(vec.x * std::cos(radians) - vec.y * std::sin(radians)), static_cast<T>(vec.x * std::sin(radians) + vec.y * std::cos(radians)) };
                
            assert(Auxiliry::is_equal_with_precision(vec.get_length(), tmp.get_length()));
                
            return tmp;
        }

        [[maybe_unused]] Vector2D get_rotated(double radians) const
        {
            return Vector2D::get_rotated(*this, radians);
        }

        [[maybe_unused]] Vector2D& rotate(double radians)
        {
            return *this = Vector2D::get_rotated(*this, radians);
        }


        T x;
        T y;
    };

    template<typename T>
    constexpr Vector2D<T> operator-(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() -= std::declval<Vector2D<T> const&>()))
    {
        return Vector2D<T>{ lhs } -= rhs;
    }
        
    template<typename T>
    constexpr Vector2D<T> operator+(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() += std::declval<Vector2D<T> const&>()))
    {
        return Vector2D<T>{ lhs } += rhs;
    }
    
    template<typename T>
    constexpr Vector2D<T> operator/(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() /= std::declval<Vector2D<T> const&>()))
    {
        return Vector2D<T>{ lhs } /= rhs;
    }

    template<typename T>
    constexpr Vector2D<T> operator/(Vector2D<T> const& lhs, T const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() /= std::declval<T const&>()))
    {
        return Vector2D<T>{ lhs } /= rhs;
    }

    template<typename T>
    constexpr Vector2D<T> operator*(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() *= std::declval<Vector2D<T> const&>()))
    {
        return Vector2D<T>{ lhs } *= rhs;
    }

    template<typename T>
    constexpr Vector2D<T> operator*(Vector2D<T> const& lhs, T const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() *= std::declval<T const&>()))
    {
        return Vector2D{ lhs } *= rhs;
    }

    template<typename T>
    constexpr Vector2D<T> operator*(T const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T>&>() *= std::declval<T const&>()))
    {
        return Vector2D{ rhs } *= lhs;
    }

    template<typename T>
    constexpr bool operator==(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<T const&>() == std::declval<T const&>()))
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    template<typename T>
    constexpr bool operator!=(Vector2D<T> const& lhs, Vector2D<T> const& rhs) noexcept(noexcept(std::declval<Vector2D<T> const&>() == std::declval<Vector2D<T> const&>()))
    {
        return !(lhs == rhs);
    }
}
