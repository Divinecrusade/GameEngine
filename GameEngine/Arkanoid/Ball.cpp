#include "Ball.hpp"
#include <cassert>
#include <cmath>


Ball::Ball(GameEngine::Geometry::Vector2D<int> init_pos, GameEngine::Geometry::Vector2D<float> init_dir, float init_speed) noexcept
:
cur_pos{ init_pos },
cur_speed{ init_speed }
{ 
    change_direction(init_dir);
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
    cur_vel *= ((cur_speed + a) / cur_speed);
    cur_speed += a;
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
    cur_vel = dir.get_normalized() * cur_speed;
    if (std::fabs(cur_vel.x) <= MIN_ABS_X_VEL) cur_vel.x = (std::signbit(cur_vel.x) ? -1.f : 1.f) * MIN_ABS_X_VEL;
    if (std::fabs(cur_vel.y) <= MIN_ABS_Y_VEL) cur_vel.y = (std::signbit(cur_vel.y) ? -1.f : 1.f) * MIN_ABS_Y_VEL;
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
