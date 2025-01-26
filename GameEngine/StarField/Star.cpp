#include "Star.hpp"


Star::Star(Vec2f const& init_pos, float init_outer_radius, int init_flares_count, GameEngine::Colour init_border_colour) noexcept
:
pos{ init_pos },
outer_radius{ init_outer_radius },
flares_count{ init_flares_count },
border_colour{ init_border_colour }
{
    assert(flares_count > 1);
}

GameEngine::Shape Star::get_shape() const
{
    std::vector<Vec2f> shape_vertices{ };
    shape_vertices.reserve(flares_count);
    shape_vertices.emplace_back(outer_radius, 0.f);

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
        vertex += pos;
    }

    return GameEngine::Shape{ shape_vertices };
}

GameEngine::Colour Star::get_colour() const noexcept
{
    return border_colour;
}
