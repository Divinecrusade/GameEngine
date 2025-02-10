#include "WorldTransformer.hpp"

namespace GameEngine
{
    Shape WorldTransformer::transform(Shape model) const noexcept
    {
        return Shape::transform(std::move(model), 
            Geometry::Transformations2D::get_translation(accumulated_translation.x, accumulated_translation.y) * 
            Geometry::Transformations2D::get_scaling(accumulated_scaling, accumulated_scaling) *
            Geometry::Transformations2D::get_rotation(accumulated_rotation)          
        );
    }

    void WorldTransformer::translate(Vec2f const& delta_translation) noexcept
    {
        accumulated_translation += delta_translation;
    }

    void WorldTransformer::scale(float ratio) noexcept
    {
        accumulated_scaling *= ratio;
    }

    void WorldTransformer::rotate(radian angle) noexcept
    {
        accumulated_rotation += angle;
    }
}
