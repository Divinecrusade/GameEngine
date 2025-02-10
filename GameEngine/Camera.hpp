#pragma once

#include "Rectangle2D.hpp"
#include "WorldTransformer.hpp"
#include "Shape.hpp"


namespace GameEngine
{
    class Camera final
    {
    public:

        using Rec2f = Geometry::Rectangle2D<float>;
        using Vec2f = Geometry::Vector2D<float>;

    public:

        Camera(Rec2f const& init_camera_area, WorldTransformer& wt, float init_min_zoom, float init_max_zoom) noexcept;

        void move_by(Vec2f delta_pos) noexcept;

        float get_zoom() const noexcept;
        void  set_zoom(float new_zoom_factor) noexcept;

        bool contains(GameEngine::Shape const& object) const noexcept;

    private:

        Vec2f accumulated_translation{ 0.f, 0.f };
        float accumulated_scaling{ 1.f };

        Rec2f const camera_area;
        WorldTransformer& wt;

        float const min_zoom;
        float const max_zoom;
    };
}