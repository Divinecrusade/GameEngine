#include "Ball.hpp"

Ball::Ball(GameEngine::Geometry::Vector2D<int> init_pos, GameEngine::Geometry::Vector2D<float> init_vel)
:
cur_pos{ init_pos },
cur_vel{ init_vel }
{
}

void Ball::draw(GameEngine::Interfaces::IGraphics2D& gfx) const
{
    gfx.fill_ellipse(cur_pos, RADIUS, RADIUS, c);
}

void Ball::update(float dt)
{
    cur_pos += cur_vel * dt;
}

void Ball::accelerate(float a)
{
    cur_vel += GameEngine::Geometry::Vector2D<float>{ a, a };
}

GameEngine::Geometry::Rectangle2D<int> Ball::get_collision_box() const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(cur_pos, RADIUS, RADIUS);
}

GameEngine::Geometry::Vector2D<float> Ball::get_velocity() const
{
    return cur_vel;
}

void Ball::move_by(GameEngine::Geometry::Vector2D<int> const& dpos)
{
    cur_pos += dpos;
}

void Ball::inverse_x()
{
    cur_vel.x *= -1.f;
}

void Ball::inverse_y()
{
    cur_vel.y *= -1.f;
}
