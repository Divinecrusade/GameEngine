#pragma once

#include <Vector2D.hpp>
#include <vector>


namespace GameEngine
{
    class Shape
    {
    public:

        using Vec2f = Geometry::Vector2D<float>;

    public:

        Shape(std::vector<Vec2f> init_vertices) noexcept;

        std::vector<Vec2f> const& get_vertices() const noexcept;

    private:

        std::vector<Vec2f> vertices{ };
    };
}
