#pragma once

#include "Shape.hpp"
#include "Transformations2D.hpp"


namespace GameEngine
{
    class WorldTransformer final
    {
    public:

        using Vec2f = Geometry::Vector2D<float>;

    public:

        Shape transform(Shape model) const noexcept;

        void  translate(Vec2f const& delta_translation) noexcept;
        void  scale(float ratio) noexcept;

    private:

        Vec2f accumulated_translation{ 0.f, 0.f };
        float accumulated_scaling{ 1.f };
    };
}