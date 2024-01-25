#include "Paddle.hpp"


Paddle::Paddle(GameEngine::Geometry::Vector2D<int> init_pos, float init_speed, int init_half_width)
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

void Paddle::draw(GameEngine::Interfaces::IGraphics2D& gfx)
{
    GameEngine::Geometry::Rectangle2D<int> const pad_rect{ GameEngine::Geometry::Rectangle2D<int>::get_from_center(cur_pos, cur_half_width, HALF_HEIGHT) };
    gfx.fill_rectangle(pad_rect, MAIN_COLOUR);
    GameEngine::Geometry::Rectangle2D<int> const left_wing{ pad_rect.left, pad_rect.left + WING_WIDTH, pad_rect.bottom, pad_rect.top };
    GameEngine::Geometry::Rectangle2D<int> const right_wing{ pad_rect.right - WING_WIDTH, pad_rect.right, pad_rect.bottom, pad_rect.top };
    gfx.fill_rectangle(left_wing, WINGS_COLOUR);
    gfx.fill_rectangle(right_wing, WINGS_COLOUR);
}

void Paddle::update(float dt)
{
    cur_pos += GameEngine::Geometry::Vector2D<float>{ static_cast<float>(cur_dir), 0.f } * cur_speed * dt;
}

Paddle::Direction Paddle::get_direction() const noexcept
{
    return cur_dir;
}

void Paddle::set_direction(Direction dir)
{
    cur_dir = dir;
}

void Paddle::accelerate(float a)
{
    cur_speed += a;
}

GameEngine::Geometry::Rectangle2D<int> Paddle::get_collision() const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(cur_pos, cur_half_width, HALF_HEIGHT);
}

void Paddle::move_by(GameEngine::Geometry::Vector2D<int> const& dpos) noexcept
{
    assert(dpos.y == 0);

    cur_pos += dpos;
}
