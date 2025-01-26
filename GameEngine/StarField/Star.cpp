#include "Star.hpp"


Star::Star(Vec2f const& center, float outer_radius, int flares_count, GameEngine::Colour init_border_colour)
:
border_colour{ init_border_colour }
{
    assert(flares_count > 1);

    constexpr float OUTER_RADIUS_MODIFIER{ 2.f };

    model.reserve(flares_count);
    model.emplace_back(outer_radius, 0.f);

    float const delta_phi{ static_cast<float>(std::numbers::pi) / static_cast<float>(flares_count) };
    for (int i{ 1 }; i != flares_count * 2; ++i)
    {
        Vec2f const& prev_point{ model.back() };
        Vec2f cur_point
        {
            prev_point.x* std::cosf(delta_phi) - prev_point.y * std::sinf(delta_phi),
            prev_point.x* std::sinf(delta_phi) + prev_point.y * std::cosf(delta_phi)
        };

        if (i % 2 == 0) // inner flare
        {
            cur_point /= OUTER_RADIUS_MODIFIER;
        }
        else            // outer flare
        {
            cur_point *= OUTER_RADIUS_MODIFIER;
        }

        model.emplace_back(std::move(cur_point));
    }
}
