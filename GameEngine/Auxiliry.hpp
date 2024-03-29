#pragma once

#include <numbers>
#include <cmath>


namespace GameEngine
{
    namespace Geometry
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
            inline bool is_equal_with_precision(double a, double b) noexcept
            {
                return std::fabs(a - b) <= PRECISION;
            }

            template <std::integral T> 
            constexpr inline bool signbit(T const& val) noexcept 
            {
                return val < static_cast<T>(0);
            }
        }
    }
}