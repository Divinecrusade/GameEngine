#include "Shape.hpp"


namespace GameEngine
{
    Shape::Shape(std::vector<Vec2f> init_vertices) noexcept
    :
    vertices{ std::move(init_vertices) }
    { }

    Shape Shape::transform(Shape object, Geometry::Transformations2D::Mat2f const& transformation) noexcept
    {
        for (auto& vertex : object.vertices)
        {
            vertex = Geometry::Transformations2D::to_vector_form(Geometry::Transformations2D::to_matrix_form(vertex) * transformation);
        }
        return object;
    }

    std::vector<Shape::Vec2f> const& Shape::get_vertices() const noexcept
    {
        return vertices;
    }
}
