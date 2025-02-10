#include "Camera.hpp"


namespace GameEngine
{
    Camera::Camera(Rec2f const& init_camera_area, WorldTransformer& wt, float init_min_zoom, float init_max_zoom) noexcept
    :
    camera_area{ init_camera_area },
    wt{ wt },
    min_zoom{ init_min_zoom },
    max_zoom{ init_max_zoom }
    { }

    void Camera::move_by(Vec2f delta_pos) noexcept
    {
        accumulated_translation += delta_pos;
        wt.translate(delta_pos * -1.f);
    }

    float Camera::get_zoom() const noexcept
    {
        return accumulated_scaling;
    }

    void Camera::set_zoom(float new_zoom_factor) noexcept
    {
        if (new_zoom_factor < min_zoom) new_zoom_factor = min_zoom;
        else if (new_zoom_factor > max_zoom) new_zoom_factor = max_zoom;

        wt.scale(new_zoom_factor / accumulated_scaling);
        accumulated_scaling = new_zoom_factor;
    }
    
    bool Camera::contains(GameEngine::Shape const& object) const noexcept
    {
        for (auto vertex : object.get_vertices())
        {
            vertex -= accumulated_translation;
            vertex *= accumulated_scaling;
            if (Rec2f::contains(vertex, camera_area)) return true;
            //if (!Rec2f::contains(vertex, camera_area)) return false;
        }
        return false;
        //return true;
    }
}
