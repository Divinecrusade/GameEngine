#pragma once

#include "Auxiliry.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>
#include <cmath>
#include <cassert>


namespace GameEngine
{ 
    namespace Geometry
    {
        template<typename T>
        requires (std::is_arithmetic<T>() == true)
        struct Vector2D final
        {
            constexpr Vector2D() noexcept = default;
            constexpr Vector2D(T const& x, T const& y)
            :
            x{ x },
            y{ y }
            { }
            constexpr Vector2D(Vector2D const&) = default;
            Vector2D(Vector2D&&) = default;
            constexpr Vector2D& operator=(Vector2D const&) = default;
            Vector2D& operator=(Vector2D&&) = default;

            ~Vector2D() = default;

            template<typename F>
            constexpr Vector2D<T>(Vector2D<F> const& vec)
            :
            x{ static_cast<T>(vec.x) },
            y{ static_cast<T>(vec.y) }
            { }

            template<typename F>
            constexpr Vector2D<T>& operator=(Vector2D<F> const& vec)
            {
                x = static_cast<T>(vec.x);
                y = static_cast<T>(vec.y);
                return *this;
            }

            constexpr Vector2D& operator-=(Vector2D const& rhs)
            {
                x -= rhs.x;
                y -= rhs.y;
                return *this;
            }

            constexpr Vector2D& operator+=(Vector2D const& rhs)
            {
                x += rhs.x;
                y += rhs.y;
                return *this;
            }

            constexpr Vector2D& operator/=(Vector2D const& rhs)
            {
                x /= rhs.x;
                y /= rhs.y;
                return *this;
            }

            constexpr Vector2D& operator/=(T const& rhs)
            {
                x /= rhs;
                y /= rhs;
                return *this;
            }

            constexpr Vector2D& operator*=(Vector2D const& rhs)
            {
                x *= rhs.x;
                y *= rhs.y;
                return *this;
            }

            constexpr Vector2D& operator*=(T const& rhs)
            {
                x *= rhs;
                y *= rhs;
                return *this;
            }

            T get_square_length() const
            {
                return Vector2D<T>::get_square_length(*this);
            }

            static T get_square_length(Vector2D const& vec)
            {
                return vec.x * vec.x + vec.y * vec.y;
            }

            static double get_length(Vector2D const& vec)
            {
                return std::sqrt(get_square_length(vec));
            }

            double get_length() const
            {
                return Vector2D::get_length(*this);
            }

            static Vector2D get_normalized(Vector2D const& vec)
            {
                double const len{ get_length(vec) };
                if (!Auxiliry::is_equal_with_precision(len, 0.))
                {
                    return vec / static_cast<T>(len);
                }
                return vec;
            }

            Vector2D get_normalized() const
            {
                return Vector2D<T>::get_normalized(*this);
            }

            Vector2D& normalize()
            {
                return *this = get_normalized(*this);
            }

            static Vector2D get_rotated(Vector2D const& vec, double radians)
            {
                Vector2D tmp{ static_cast<T>(vec.x * std::cos(radians) - vec.y * std::sin(radians)), static_cast<T>(vec.x * std::sin(radians) + vec.y * std::cos(radians)) };
                
                assert(Auxiliry::is_equal_with_precision(vec.get_length(), tmp.get_length()));
                
                return tmp;
            }

            Vector2D get_rotated(double radians) const
            {
                return Vector2D::get_rotated(*this, radians);
            }

            Vector2D& rotate(double radians)
            {
                return *this = Vector2D::get_rotated(*this, radians);
            }


            T x;
            T y;
        };

        template<typename T>
        constexpr Vector2D<T> operator-(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } -= rhs;
        }
        
        template<typename T>
        constexpr Vector2D<T> operator+(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } += rhs;
        }
    
        template<typename T>
        constexpr Vector2D<T> operator/(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } /= rhs;
        }

        template<typename T>
        constexpr Vector2D<T> operator/(Vector2D<T> const& lhs, T const& rhs)
        {
            return Vector2D<T>{ lhs } /= rhs;
        }

        template<typename T>
        constexpr Vector2D<T> operator*(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } *= rhs;
        }

        template<typename T>
        constexpr Vector2D<T> operator*(Vector2D<T> const& lhs, T const& rhs)
        {
            return Vector2D{ lhs } *= rhs;
        }

        template<typename T>
        constexpr Vector2D<T> operator*(T const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D{ rhs } *= lhs;
        }

        template<typename T>
        constexpr bool operator==(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        template<typename T>
        constexpr bool operator!=(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return !(lhs == rhs);
        }
    }
}
