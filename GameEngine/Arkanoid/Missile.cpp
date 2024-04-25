#include "Missile.hpp"


Missile::Missile(Vec2i const& init_pos, float init_speed, GameEngine::Surface const& sprite, GameEngine::Colour chroma)
:
cur_pos  { init_pos },
cur_speed{ init_speed },
cur_vel  { DIR * cur_speed },
sprite{ sprite },
chroma{ chroma }
{ }

Missile::Missile(Missile const& other) noexcept
:
destroyed{ other.destroyed },
cur_pos  { other.cur_pos },
cur_speed{ other.cur_speed },
cur_vel  { other.cur_vel },
sprite{ other.sprite },
chroma{ other.chroma }
{ }

Missile::Missile(Missile&& other_tmp) noexcept
:
sprite{ other_tmp.sprite },
chroma{ other_tmp.chroma }
{
    swap(std::move(other_tmp));
}

void Missile::draw(GameEngine::Interfaces::IGraphics2D & gfx, GameEngine::Geometry::Rectangle2D<int> const& clip) const
{
    gfx.draw_sprite_excluding_color(Vec2i{ cur_pos.x - static_cast<int>(sprite.get_width() / 2U), cur_pos.y - static_cast<int>(sprite.get_height() / 2U) }, sprite, chroma, clip);
}

void Missile::update(float dt)
{
    cur_pos += dt * cur_vel;
}

Missile::Vec2i Missile::get_pos() const noexcept
{
    return cur_pos;
}

bool Missile::is_collided_with(PlayField const& border) const noexcept
{
    return border.get_collision_box().bottom <= (cur_pos.y + COLLISION_HALF_HEIGHT);
}

bool Missile::is_collided_with(Ball const& ball) const noexcept
{
    return Rec2i::get_from_center(cur_pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT).is_colided_with(ball.get_collision_box());
}

bool Missile::is_collided_with(Paddle const& padd) const noexcept
{
    return Rec2i::get_from_center(cur_pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT).is_colided_with(padd.get_collision_box());
}

Missile::Rec2i Missile::get_collision_box() const
{
    return Rec2i::get_from_center(cur_pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT);
}

void Missile::swap(Missile&& other_tmp) noexcept
{
    std::swap(destroyed, other_tmp.destroyed);
    std::swap(cur_pos, other_tmp.cur_pos);
    std::swap(cur_speed, other_tmp.cur_speed);
    std::swap(cur_vel, other_tmp.cur_vel);
}