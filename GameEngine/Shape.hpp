#pragma once

#include <Vector2D.hpp>
#include <Transformations2D.hpp>
#include <vector>


namespace GameEngine
{
    class Shape
    {
    public:

        using Vec2f = Geometry::Vector2D<float>;

    public:

        Shape() = delete;
        Shape(Shape const&) = delete;
        Shape(Shape&&)      = default;
        Shape(std::vector<Vec2f> init_vertices) noexcept;

        Shape& operator=(Shape const&) = delete;
        Shape& operator=(Shape&&)      = delete;

        ~Shape() = default;

        std::vector<Vec2f> const& get_vertices() const noexcept;

        static Shape transform(Shape object, Geometry::Transformations2D::Mat2f const& transformation) noexcept;

    private:

        std::vector<Vec2f> vertices{ };
    };
}
