#include "Paddle.hpp"


Paddle::Paddle(Vec2i init_pos, float init_speed, int init_half_width) noexcept
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
    Rec2i const pad_rect{ Rec2i::get_from_center(cur_pos, cur_half_width, HALF_HEIGHT) };
    gfx.fill_rectangle(pad_rect, MAIN_COLOUR);
    Rec2i const left_wing{ pad_rect.left, pad_rect.left + WING_WIDTH, pad_rect.bottom, pad_rect.top };
    Rec2i const right_wing{ pad_rect.right - WING_WIDTH, pad_rect.right, pad_rect.bottom, pad_rect.top };
    gfx.fill_rectangle(left_wing, WINGS_COLOUR);
    gfx.fill_rectangle(right_wing, WINGS_COLOUR);
}

void Paddle::update(float dt) noexcept
{
    cur_pos += Vec2f{ static_cast<float>(cur_dir), 0.f } * cur_speed * dt;
}

Paddle::Direction Paddle::get_direction() const noexcept
{
    return cur_dir;
}

void Paddle::set_direction(Direction dir) noexcept
{
    cur_dir = dir;
}

void Paddle::move_by(Vec2i const& dpos) noexcept
{
    assert(dpos.y == 0);

    cur_pos += dpos;
}

void Paddle::accelerate(float a) noexcept
{
    cur_speed += a;
}

void Paddle::deflect(Ball& ball)
{
    assert(is_collided_with(ball));

    if (!cooldown)
    {
        CollisionEdge const edge{ process(ball) };
        deflect(ball, edge);

        cooldown = true;
    }
}

Paddle::Rec2i Paddle::get_collision_box() const noexcept
{
    return Rec2i::get_from_center(cur_pos, cur_half_width, HALF_HEIGHT);
}

bool Paddle::is_cooldowned() const noexcept
{
    return cooldown;
}

void Paddle::reset_cooldown() noexcept
{
    cooldown = false;
}

Paddle::CollisionEdge Paddle::process(Ball& ball) const noexcept
{
    assert(is_collided_with(ball));
    assert(!cooldown);

    CollisionEdge edge{ };
    
    auto const ball_collision_box{ ball.get_collision_box() };
    auto const ball_center{ ball.get_collision_box().get_center() };
    auto const padd_collision_box{ get_collision_box() };
    if (ball_center.x >= padd_collision_box.left && ball_center.x <= padd_collision_box.right)
    {
        if (ball.get_velocity().y > 0.f) edge = CollisionEdge::TOP;
        else edge = CollisionEdge::BOTTOM;
    }
    else 
    {
        if (ball.get_velocity().x > 0.f) edge = CollisionEdge::LEFT;
        else edge = CollisionEdge::RIGHT;
    }

    return edge;
}

void Paddle::deflect(Ball& ball, CollisionEdge edge) const
{
    assert(!cooldown);

    switch (edge)
    {
        case CollisionEdge::BOTTOM:
        case CollisionEdge::TOP:
        
            ball.change_direction(calculate_deflect_direction(edge, ball.get_collision_box().get_center().x - cur_pos.x, (ball.get_velocity().x > 0.f ? Direction::RIGHT : Direction::LEFT)));

        break;

        case CollisionEdge::LEFT:
        case CollisionEdge::RIGHT:
            
            ball.inverse_x();

        break;
    }
}

Paddle::Vec2f Paddle::calculate_deflect_direction(CollisionEdge edge, double dL, Direction ball_direction) const
{
    assert(edge == CollisionEdge::BOTTOM || edge == CollisionEdge::TOP);
    assert(ball_direction == Direction::LEFT || ball_direction == Direction::RIGHT);

    Vec2f new_dir{ 0, (edge == CollisionEdge::TOP ? -1.f : 1.f) };
    Direction padd_collised_part{ dL < 0.f ? Direction::RIGHT : Direction::LEFT };
    double const abs_dL{ std::fabs(dL) };
    double const deflect_angle
    {
        ((padd_collised_part == ball_direction) ? (ball_direction == Direction::LEFT ? 1. : -1.) : (ball_direction == Direction::LEFT ? -1. : 1.))
        *
        (abs_dL <= cur_half_width * MIN_DEFLECT_ZONE_RATIO ?
        MIN_ANGLE_DEFLECT
        :
        min(MAX_ANGLE_DEFLECT, (MIN_ANGLE_DEFLECT + (MAX_ANGLE_DEFLECT - MIN_ANGLE_DEFLECT) * (abs_dL / cur_half_width))))
    };
    new_dir.rotate(deflect_angle);

    return new_dir;
}
