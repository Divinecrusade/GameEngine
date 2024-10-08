#include "PlayField.hpp"


PlayField::PlayField(Rec2i const& size_and_location) noexcept
:
collision_frame{ size_and_location }
{
    assert(collision_frame.left >= 0);
    assert(collision_frame.right >= 0);
    assert(collision_frame.bottom >= 0);
    assert(collision_frame.top >= 0);
    assert(collision_frame.left < collision_frame.right);
    assert(collision_frame.top < collision_frame.bottom);
}

void PlayField::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const&) const
{
    gfx.draw_rectangle(collision_frame.get_expanded(inner_border_thickness), inner_border_thickness, inner_border_colour);
    gfx.draw_rectangle(collision_frame.get_expanded(inner_border_thickness + outer_border_thickness), outer_border_thickness, outer_border_colour);
    gfx.draw_line(Vec2i{ collision_frame.left,  collision_frame.bottom },
                  Vec2i{ collision_frame.right, collision_frame.bottom }, 
                  lose_zone_line_thickness, lose_zone_colour);
}

bool PlayField::is_in_field(GameEngine::Abstract::Collidable const& obj) const noexcept
{
    return collision_frame.contains(obj.get_collision_box());
}

bool PlayField::is_in_lose_zone(Ball const& ball) const noexcept
{
    return ball.get_collision_box().is_colided_with(Rec2i{{collision_frame.left, collision_frame.bottom}, {collision_frame.right, collision_frame.bottom}});
}

void PlayField::handle_collision(Paddle& pad) const noexcept
{
    assert(!is_in_field(pad));

    auto const rect_pad{ pad.get_collision_box() };
    pad.move_by(GameEngine::Geometry::Vector2D<int>{ (rect_pad.left < collision_frame.left ? collision_frame.left - rect_pad.left : collision_frame.right - rect_pad.right), 0 });
}

void PlayField::handle_collision(Ball& ball) const noexcept
{
    assert(!is_in_field(ball));

    auto const ball_collision_box{ ball.get_collision_box() };
    if (ball_collision_box.left < collision_frame.left)
    {
        ball.move_by(GameEngine::Geometry::Vector2D<int>{collision_frame.left - ball_collision_box.left, 0});
        ball.inverse_x();
    }
    else if (ball_collision_box.right > collision_frame.right)
    {
        ball.move_by(GameEngine::Geometry::Vector2D<int>{collision_frame.right - ball_collision_box.right, 0});
        ball.inverse_x();
    }
    else if (ball_collision_box.top < collision_frame.top)
    {
        ball.move_by(GameEngine::Geometry::Vector2D<int>{0, collision_frame.top - ball_collision_box.top});
        ball.inverse_y();
    }
    else
    {
        ball.move_by(GameEngine::Geometry::Vector2D<int>{0, collision_frame.bottom - ball_collision_box.bottom});
        ball.inverse_y();
    }
}

PlayField::Rec2i PlayField::get_collision_box() const noexcept
{
    return collision_frame;
}
