#pragma once

#include <numbers>
#include <cmath>


namespace GameEngine::Geometry
{
    namespace Literals
    {
        inline consteval double operator ""_deg(long double angle)
        {
            return static_cast<double>(angle / 180.L) * std::numbers::pi;
        }
        inline consteval double operator ""_percent(long double percentage)
        {
            return static_cast<double>(percentage / 100.L);
        }
    }

    namespace Auxiliry
    {            
        static constexpr double PRECISION{ 1e-5 };
        [[maybe_unused]] constexpr inline bool is_equal_with_precision(double a, double b) noexcept
        {
            return ((a > b) ? a : b) - ((a < b) ? a : b) <= PRECISION;
        }

        template <std::integral T> 
        [[maybe_unused]] constexpr inline bool signbit(T const& val) noexcept
        {
            return val < static_cast<T>(0);
        }
    }
}