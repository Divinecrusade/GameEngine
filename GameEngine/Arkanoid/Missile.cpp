#include "Missile.hpp"
#include <utility>


Missile::Missile(Vec2i const& init_pos, float init_speed, GameEngine::SurfaceView sprite, GameEngine::Colour chroma) noexcept
:
cur_pos  { init_pos },
cur_speed{ init_speed },
cur_vel  { DIR * cur_speed },
sprite   { sprite, chroma }
{ }

void Missile::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(!is_destroyed());
    assert(clipping_area.has_value());

    gfx.draw_sprite_excluding_color(Vec2i{ cur_pos.x - static_cast<int>(sprite.first.get_width() / 2U), cur_pos.y - static_cast<int>(sprite.first.get_height() / 2U) }, sprite.first, sprite.second, *clipping_area);
}

void Missile::update(float dt) noexcept
{
    assert(!is_destroyed());

    cur_pos += dt * cur_vel;
}

Missile::Rec2i Missile::get_collision_box() const noexcept
{
    return Rec2i::get_from_center(cur_pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT);
}

Missile::Vec2i Missile::get_pos() const noexcept
{
    return cur_pos;
}

void Missile::destroy() noexcept
{
    destroyed = true;
}

bool Missile::is_destroyed() const noexcept
{
    return destroyed;
}