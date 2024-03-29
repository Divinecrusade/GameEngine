#include "Ball.hpp"
#include <cassert>

Ball::Ball(GameEngine::Geometry::Vector2D<int> init_pos, GameEngine::Geometry::Vector2D<float> init_vel) noexcept
:
cur_pos{ init_pos },
cur_vel{ init_vel }
{
}

void Ball::draw(GameEngine::Interfaces::IGraphics2D& gfx) const
{
    gfx.fill_ellipse(cur_pos, RADIUS, RADIUS, c);
}

void Ball::update(float dt) noexcept
{
    cur_pos += cur_vel * dt;
}

void Ball::accelerate(float a) noexcept
{
    cur_vel += GameEngine::Geometry::Vector2D<float>{ a, a };
}

GameEngine::Geometry::Rectangle2D<int> Ball::get_collision_box() const noexcept
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(cur_pos, RADIUS, RADIUS);
}

GameEngine::Geometry::Vector2D<int> Ball::get_center() const noexcept
{
    return cur_pos;
}

GameEngine::Geometry::Vector2D<float> Ball::get_velocity() const noexcept
{
    return cur_vel;
}

void Ball::change_direction(GameEngine::Geometry::Vector2D<float> const& dir)
{
    auto cur_magnitude{ cur_vel.get_length() };

    cur_vel = dir.get_normalized() * static_cast<float>(cur_magnitude);
    if (cur_vel.y > 0.f && cur_vel.y < MIN_VEL) cur_vel.y = MIN_VEL;
    else if (cur_vel.y < 0.f && cur_vel.y > -MIN_VEL) cur_vel.y = -MIN_VEL;
    if (cur_vel.x > 0.f && cur_vel.x < MIN_VEL) cur_vel.x = MIN_VEL;
    else if (cur_vel.x < 0.f && cur_vel.x > -MIN_VEL) cur_vel.x = -MIN_VEL;
}

void Ball::move_by(GameEngine::Geometry::Vector2D<int> const& dpos) noexcept
{
    cur_pos += dpos;
}

void Ball::inverse_x() noexcept
{
    cur_vel.x *= -1.f;
}

void Ball::inverse_y() noexcept
{
    cur_vel.y *= -1.f;
}
