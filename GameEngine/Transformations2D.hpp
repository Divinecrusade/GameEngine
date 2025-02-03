#pragma once

#include "Vector2D.hpp"
#include "Matrix.hpp"


namespace GameEngine::Geometry::Transformations2D
{
    using Vec2f = Vector2D<float>;
    using Mat2f = Matrices::Matrix<3U, 3U, float>;
    using MatVec2D = Matrices::Matrix<1U, 3U, float>;

    static constexpr float k{ 1.f };

    inline MatVec2D to_matrix_form(Vec2f const& v) noexcept
    {
        return std::array{ v.x, v.y, k };
    }

    inline Vec2f to_vector_form(MatVec2D const& m) noexcept
    {
        auto const flattern_view{ m.flattern() };
        assert(Auxiliry::is_equal_with_precision(flattern_view[2U], k));
        return Vec2f{ flattern_view[0U], flattern_view[1U] };
    }

    inline void apply(Vec2f& lhs, Mat2f const& rhs) noexcept
    {
        lhs = to_vector_form(to_matrix_form(lhs) * rhs);
    }

    inline Mat2f get_rotation(float phi, float r = 1.f) noexcept
    {
        return std::array
        { 
            r * std::cosf(phi), -r * std::sinf(phi), 0.f,
            r * std::sinf(phi), r * cosf(phi), 0.f,
            0.f, 0.f, k
        };
    }
    
    inline Mat2f get_translation(float dx, float dy) noexcept
    {
        return std::array
        {
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            dx, dy, 1.f
        };
    }

    inline Mat2f get_scaling(float mx, float my) noexcept
    {
        return std::array
        {
            mx, 0.f, 0.f,
            0.f, my, 0.f,
            0.f, 0.f, 1.f
        };
    }
}