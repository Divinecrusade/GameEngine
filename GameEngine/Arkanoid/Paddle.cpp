#include "Paddle.hpp"


Paddle::Paddle(GameEngine2D::Geometry::Vector2D<int> init_pos, float init_speed, int init_half_width)
:
cur_pos{ init_pos },
cur_speed{ init_speed },
cur_half_width{ init_half_width }
{ 
    assert(init_pos.x >= 0);
    assert(init_pos.y >= 0);
    assert(init_speed > 0.f);
    assert(init_half_width > 0);
}

void Paddle::draw(GameEngine2D::Interfaces::IGraphics2D& gfx)
{
    gfx.fill_rectangle(GameEngine2D::Geometry::Rectangle2D<int>::get_from_center(cur_pos, cur_half_width, HALF_HEIGHT), MAIN_COLOUR);
}

void Paddle::update(float dt)
{
    cur_pos += GameEngine2D::Geometry::Vector2D<float>{ static_cast<float>(cur_dir), 0.f } * cur_speed * dt;
}

void Paddle::set_direction(Direction dir)
{
    cur_dir = dir;
}

void Paddle::accelerate(float a)
{
    cur_speed += a;
}
