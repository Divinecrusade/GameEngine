#include "LifeCounter.hpp"


LifeCounter::LifeCounter(int min_value, int max_value, GameEngine::Geometry::Vector2D<int> const& left_top_pos, GameEngine::SurfaceView sprite, GameEngine::Colour chroma) noexcept
:
min_value{ min_value },
max_value{ max_value },
cur_value{ max_value },
left_top_pos{ left_top_pos },
sprite{ sprite, chroma }
{ 
    assert(min_value <= max_value);
    assert(cur_value >= min_value && cur_value <= max_value);
}

bool LifeCounter::is_ended() const noexcept
{
    return cur_value == min_value;
}

int LifeCounter::get_cur_n_lives() const noexcept
{
    return cur_value;
}

void LifeCounter::reset() noexcept
{
    cur_value = max_value;
}

void LifeCounter::decrease() noexcept
{
    assert(cur_value >= min_value);
    --cur_value;
}

void LifeCounter::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(clipping_area.has_value());

    for (int i{ min_value - min_value }; i < cur_value - min_value; ++i)
    {
        gfx.draw_sprite_excluding_color
        (
            { static_cast<int>(left_top_pos.x + (sprite.first.get_width() + MARGIN_RIGHT) * i), left_top_pos.y },
            sprite.first,
            sprite.second,
            *clipping_area
        );
    }
}