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
                return angle * std::numbers::pi / 180;
            }
            inline consteval double operator ""_percent(long double percentage)
            {
                return percentage / 100;
            }
        }

        namespace Auxiliry
        {            
            static constexpr double PRECISION{ 1e-5 };
            inline bool is_equal_with_precision(double a, double b)
            {
                return std::fabs(a - b) <= PRECISION;
            }
        }
    }
}