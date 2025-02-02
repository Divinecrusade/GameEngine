#include "Shape.hpp"


namespace GameEngine
{
    Shape::Shape(std::vector<Vec2f> init_vertices) noexcept
    :
    vertices{ std::move(init_vertices) }
    { }

    std::vector<Shape::Vec2f> const& Shape::get_vertices() const noexcept
    {
        return vertices;
    }
}
