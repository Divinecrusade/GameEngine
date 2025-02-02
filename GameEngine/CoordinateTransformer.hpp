#pragma once

#include "Shape.hpp"
#include "Rectangle2D.hpp"

#include <algorithm>
#include <ranges>


namespace GameEngine
{
    class CoordinateTransformer final
    {
    public:

        using Vec2i = Geometry::Vector2D<int>;
        using Rec2i = Geometry::Rectangle2D<int>;

    public:

        CoordinateTransformer(Rec2i const& viewport);

        std::vector<Vec2i> transform(Shape object) const;

    private:

        Vec2i const left_top_delta;
        int const viewport_halfwidth;
        int const viewport_halfheight;
    };
}