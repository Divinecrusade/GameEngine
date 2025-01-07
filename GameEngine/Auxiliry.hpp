#pragma once

#include <numbers>
#include <cmath>


namespace GameEngine::Geometry
{
    namespace Literals
    {
        inline constexpr double operator ""_deg(long double angle) noexcept
        {
            return static_cast<double>(angle / 180.L) * std::numbers::pi;
        }
        inline constexpr double operator ""_percent(long double percentage) noexcept
        {
            return static_cast<double>(percentage / 100.L);
        }
    }

    namespace Auxiliry
    {            
        template<std::floating_point T>
        [[maybe_unused]] [[nodiscard("Pure function")]] constexpr inline bool is_equal_with_precision(T a, T b, T precision = (T)1e-5) noexcept
        {
            return ((a > b) ? a : b) - ((a < b) ? a : b) <= precision;
        }

        template<std::integral T> 
        [[maybe_unused]] [[nodiscard("Pure function")]] constexpr inline bool signbit(T const& val) noexcept
        {
            return val < static_cast<T>(0);
        }
    }
}