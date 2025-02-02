#include "CoordinateTransformer.hpp"

namespace GameEngine
{
    CoordinateTransformer::CoordinateTransformer(Rec2i const& viewport)
    :
    left_top_delta{ viewport.left, viewport.top },
    viewport_halfwidth{ viewport.get_width() / 2 },
    viewport_halfheight{ viewport.get_height() / 2 }
    { }

    std::vector<CoordinateTransformer::Vec2i> CoordinateTransformer::transform(Shape object) const
    {
        std::vector<Vec2i> transformed_points{ };
        transformed_points.reserve(object.get_vertices().size());

        for (auto const& point : object.get_vertices())
        {
            int const y{ static_cast<int>(point.y) };
            transformed_points.emplace_back
            (
                static_cast<int>(point.x) + viewport_halfwidth,
                -(static_cast<int>(point.y) - viewport_halfheight)
            );
        }

        return transformed_points;
    }
}
