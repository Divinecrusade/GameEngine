#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>


namespace GameEngine
{ 
    namespace Geometry
    {
        template<typename T>
        requires (std::is_arithmetic<T>() == true)
        struct Vector2D final
        {
            Vector2D() noexcept = default;
            Vector2D(T const& x, T const& y)
            :
            x{ x },
            y{ y }
            { }
            Vector2D(Vector2D const&) = default;
            Vector2D(Vector2D&&) = default;
            Vector2D& operator=(Vector2D const&) = default;
            Vector2D& operator=(Vector2D&&) = default;

            ~Vector2D() = default;

            template<typename F>
            Vector2D<T>(Vector2D<F> const& vec)
            :
            x{ static_cast<T>(vec.x) },
            y{ static_cast<T>(vec.y) }
            { }

            template<typename F>
            Vector2D<T>& operator=(Vector2D<F> const& vec)
            {
                x = static_cast<T>(vec.x);
                y = static_cast<T>(vec.y);
                return *this;
            }

            Vector2D& operator-=(Vector2D const& rhs)
            {
                x -= rhs.x;
                y -= rhs.y;
                return *this;
            }

            Vector2D& operator+=(Vector2D const& rhs)
            {
                x += rhs.x;
                y += rhs.y;
                return *this;
            }

            Vector2D& operator/=(Vector2D const& rhs)
            {
                x /= rhs.x;
                y /= rhs.y;
                return *this;
            }

            Vector2D& operator/=(T const& rhs)
            {
                x /= rhs;
                y /= rhs;
                return *this;
            }

            Vector2D& operator*=(Vector2D const& rhs)
            {
                x *= rhs.x;
                y *= rhs.y;
                return *this;
            }

            Vector2D& operator*=(T const& rhs)
            {
                x *= rhs;
                y *= rhs;
                return *this;
            }


            static T get_square_length(Vector2D const& vec)
            {
                return vec.x * vec.x + vec.y * vec.y;
            }

            static T get_length(Vector2D const& vec)
            {
                return std::sqrt(get_square_length(vec));
            }

            static Vector2D get_normalized(Vector2D const& vec)
            {
                const T len{ get_length(vec) };
                if (len != static_cast<T>(0))
                {
                    return vec / len;
                }
                return vec;
            }

            Vector2D& normalize()
            {
                return *this = get_normalized(*this);
            }


            T x;
            T y;
        };

        template<typename T>
        Vector2D<T> operator-(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } -= rhs;
        }
        
        template<typename T>
        Vector2D<T> operator+(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } += rhs;
        }
    
        template<typename T>
        Vector2D<T> operator/(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } /= rhs;
        }

        template<typename T>
        Vector2D<T> operator/(Vector2D<T> const& lhs, T const& rhs)
        {
            return Vector2D<T>{ lhs } /= rhs;
        }

        template<typename T>
        Vector2D<T> operator*(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D<T>{ lhs } *= rhs;
        }

        template<typename T>
        Vector2D<T> operator*(Vector2D<T> const& lhs, T const& rhs)
        {
            return Vector2D{ lhs } *= rhs;
        }

        template<typename T>
        Vector2D<T> operator*(T const& lhs, Vector2D<T> const& rhs)
        {
            return Vector2D{ rhs } *= lhs;
        }

        template<typename T>
        bool operator==(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        template<typename T>
        bool operator!=(Vector2D<T> const& lhs, Vector2D<T> const& rhs)
        {
            return !(lhs == rhs);
        }
    }
}
