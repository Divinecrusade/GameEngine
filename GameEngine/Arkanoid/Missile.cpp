#include "Missile.hpp"


Missile::Missile(Vec2i const& init_pos, float init_speed, GameEngine::Surface const& sprite, GameEngine::Colour chroma)
:
cur_pos  { init_pos },
cur_speed{ init_speed },
cur_vel  { DIR * cur_speed },
sprite{ sprite },
chroma{ chroma }
{ }

Missile::Missile(Missile&& other_tmp) noexcept
    :
    sprite{ other_tmp.sprite },
    chroma{ other_tmp.chroma }
{
    swap(std::move(other_tmp));
}

Missile& Missile::operator=(Missile&& other_tmp) noexcept
{
    this->swap(std::move(other_tmp));

    return *this;
}

void Missile::draw(GameEngine::Interfaces::IGraphics2D & gfx, GameEngine::Geometry::Rectangle2D<int> const& clip) const
{
    assert(!is_destroyed());

    gfx.draw_sprite_excluding_color(Vec2i{ cur_pos.x - static_cast<int>(sprite.get_width() / 2U), cur_pos.y - static_cast<int>(sprite.get_height() / 2U) }, sprite, chroma, clip);
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

void Missile::swap(Missile&& other_tmp) noexcept
{
    std::swap(destroyed, other_tmp.destroyed);
    std::swap(cur_pos, other_tmp.cur_pos);
    std::swap(cur_speed, other_tmp.cur_speed);
    std::swap(cur_vel, other_tmp.cur_vel);
}
