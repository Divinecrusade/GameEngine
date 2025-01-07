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
        [[maybe_unused]] [[nodiscard("Pure function")]] constexpr inline bool is_equal_with_precision(double a, double b, double precision = 1e-5) noexcept
        {
            return ((a > b) ? a : b) - ((a < b) ? a : b) <= precision;
        }

        template <std::integral T> 
        [[maybe_unused]] [[nodiscard("Pure function")]] constexpr inline bool signbit(T const& val) noexcept
        {
            return val < static_cast<T>(0);
        }
    }
}