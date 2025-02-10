#include "Camera.hpp"


namespace GameEngine
{
    Camera::Camera(Rec2f const& init_camera_area, WorldTransformer& wt, float init_min_zoom, float init_max_zoom) noexcept
    :
    camera_area
    { 
        Rec2f::get_from_center_with_normal_y_axis
        (
            init_camera_area.get_center(), 
            std::max(init_camera_area.get_width(), init_camera_area.get_height()) / 2.f,
            std::max(init_camera_area.get_width(), init_camera_area.get_height()) / 2.f
        )
    },
    wt{ wt },
    min_zoom{ init_min_zoom },
    max_zoom{ init_max_zoom }
    { }

    void Camera::move(Vec2f delta_pos) noexcept
    {
        wt.translate(delta_pos * -1.f);
        accumulated_translation += delta_pos;
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

    void Camera::rotate(radian angle) noexcept
    {
        wt.rotate(angle);
        accumulated_rotation += angle;
    }
    
    bool Camera::contains(GameEngine::Shape const& object) const noexcept
    {
        for (auto vertex : object.get_vertices())
        {
            vertex -= accumulated_translation;
            vertex *= accumulated_scaling;
            vertex.rotate(-accumulated_rotation);
            //if (camera_area.contains_with_normal_y_axis(vertex)) return true;
            if (!camera_area.contains_with_normal_y_axis(vertex)) return false;
        }
        //return false;
        return true;
    }
}
