#include "Missile.hpp"
#include <utility>


Missile::Missile(Vec2i const& init_pos, float init_speed, GameEngine::SurfaceView sprite, GameEngine::Colour chroma)
:
cur_pos  { init_pos },
cur_speed{ init_speed },
cur_vel  { DIR * cur_speed },
sprite   { std::make_pair(sprite, chroma) }
{ }

Missile& Missile::operator=(Missile const& other_missile) noexcept
{
    assert(this != &other_missile);

    destroyed = other_missile.destroyed;
    cur_pos = other_missile.cur_pos;
    cur_speed = other_missile.cur_speed;
    cur_vel = other_missile.cur_vel;
    sprite = other_missile.sprite;

    return *this;
}

Missile& Missile::operator=(Missile&& other_missile_tmp) noexcept
{
    assert(this != &other_missile_tmp);

    std::swap(destroyed, other_missile_tmp.destroyed);
    std::swap(cur_pos, other_missile_tmp.cur_pos);
    std::swap(cur_speed, other_missile_tmp.cur_speed);
    std::swap(cur_vel, other_missile_tmp.cur_vel);
    std::swap(sprite, other_missile_tmp.sprite);

    return *this;
}

void Missile::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(!is_destroyed());
    assert(clipping_area.has_value());

    gfx.draw_sprite_excluding_color(Vec2i{ cur_pos.x - static_cast<int>(sprite.first.get_width() / 2U), cur_pos.y - static_cast<int>(sprite.first.get_height() / 2U) }, sprite.first, sprite.second, *clipping_area);
}

void Missile::update(float dt)
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