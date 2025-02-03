#include "Star.hpp"


Star::Star(Vec2f const& init_pos, float init_outer_radius, int init_flares_count,
    GameEngine::Colour init_border_colour, float init_rotation_speed, float init_min_size_factor,
    float init_colour_freq_factor, float init_colour_phase) noexcept
    :
    pos{ init_pos },
    outer_radius{ init_outer_radius },
    flares_count{ init_flares_count },
    base_colour{ init_border_colour },
    cur_colour{ base_colour },
    rotation_speed{ init_rotation_speed },
    min_size_factor{ init_min_size_factor },
    delta_size{ min_size_factor - 1.f },
    colour_freq_factor{ init_colour_freq_factor },
    colour_phase{ init_colour_phase }
{
    assert(flares_count > 1);
    assert(min_size_factor > 0.f && min_size_factor < 1.f);
    assert(colour_phase > 0.f && colour_phase < static_cast<float>(2. * std::numbers::pi));
}

GameEngine::Shape Star::get_shape() const
{
    std::vector<Vec2f> shape_vertices{ };
    shape_vertices.reserve(flares_count);
    shape_vertices.emplace_back(outer_radius / 2.f, 0.f);

    float const delta_phi{ static_cast<float>(std::numbers::pi) / static_cast<float>(flares_count) };
    for (int i{ 1 }; i != flares_count * 2; ++i)
    {
        Vec2f const& prev_point{ shape_vertices.back() };
        Vec2f cur_point
        {
            prev_point.x * std::cosf(delta_phi) - prev_point.y * std::sinf(delta_phi),
            prev_point.x * std::sinf(delta_phi) + prev_point.y * std::cosf(delta_phi)
        };

        if (i % 2 == 0) // inner flare
        {
            cur_point /= OUTER_RADIUS_MODIFIER;
        }
        else            // outer flare
        {
            cur_point *= OUTER_RADIUS_MODIFIER;
        }

        shape_vertices.emplace_back(std::move(cur_point));
    }

    for (auto& vertex : shape_vertices)
    {
        GameEngine::Geometry::Transformations2D::apply(vertex,
            GameEngine::Geometry::Transformations2D::get_scaling(cur_size_factor, cur_size_factor) *
            GameEngine::Geometry::Transformations2D::get_rotation(cur_rotation_angle) *
            GameEngine::Geometry::Transformations2D::get_translation(pos.x, pos.y)
        );
    }

    return GameEngine::Shape{ shape_vertices };
}

GameEngine::Colour Star::get_colour() const noexcept
{
    return cur_colour;
}

void Star::update(float dt)
{
    cur_rotation_angle += rotation_speed * dt;
    while (static_cast<double>(cur_rotation_angle) > std::numbers::pi * 2.)
    {
        cur_rotation_angle -= static_cast<float>(std::numbers::pi * 2.);
    }
    cur_size_factor += delta_size * dt;
    if (delta_size < 0.f)
    {
        if (cur_size_factor < min_size_factor)
        {
            cur_size_factor = min_size_factor;
            delta_size *= -1.f;
        }
    }
    else
    {
        if (cur_size_factor > 1.f)
        {
            cur_size_factor = 1.f;
            delta_size *= -1.f;
        }
    }
    
    time += dt;
    while (static_cast<double>(time) > std::numbers::pi * 2.)
    {
        time -= static_cast<float>(std::numbers::pi * 2.);
    }

    int const offset{ static_cast<int>(127.f * std::sinf(colour_freq_factor * time + colour_phase)) + 128 };

    cur_colour = GameEngine::Colour
    {
        static_cast<uint8_t>(std::min(static_cast<int>(base_colour[GameEngine::Colour::ComponentIndex::R]) + offset, 255)),
        static_cast<uint8_t>(std::min(static_cast<int>(base_colour[GameEngine::Colour::ComponentIndex::G]) + offset, 255)),
        static_cast<uint8_t>(std::min(static_cast<int>(base_colour[GameEngine::Colour::ComponentIndex::B]) + offset, 255))
    };
}
