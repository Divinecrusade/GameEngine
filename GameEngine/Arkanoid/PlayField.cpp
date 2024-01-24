#include "PlayField.hpp"
#include <cassert>


PlayField::PlayField(GameEngine2D::Geometry::Rectangle2D<int> const& size_and_location)
:
collision_frame{ size_and_location }
{
    assert(collision_frame.left > 0);
    assert(collision_frame.right > 0);
    assert(collision_frame.bottom > 0);
    assert(collision_frame.top > 0);
    assert(collision_frame.left < collision_frame.right);
    assert(collision_frame.top < collision_frame.bottom);
}

void PlayField::draw(GameEngine2D::Interfaces::IGraphics2D& gfx) const
{
    gfx.draw_rectangle(collision_frame.get_expanded(inner_border_thickness), inner_border_thickness, inner_border_colour);
    gfx.draw_rectangle(collision_frame.get_expanded(inner_border_thickness + outer_border_thickness), outer_border_thickness, outer_border_colour);
    gfx.draw_line(GameEngine2D::Geometry::Vector2D{ collision_frame.left, collision_frame.bottom },
                  GameEngine2D::Geometry::Vector2D{ collision_frame.right, collision_frame.bottom }, 
                  lose_zone_line_thickness, lose_zone_colour);
}

bool PlayField::is_in_field(Paddle const& pad) const
{
    return collision_frame.contains(pad.get_collision());
}

void PlayField::handle_collision(Paddle& pad) const
{
    assert(!is_in_field(pad));

    auto const rect_pad{ pad.get_collision() };
    int dx{ (rect_pad.left < collision_frame.left ? collision_frame.left - rect_pad.left : collision_frame.right - rect_pad.right) };
    pad.move_by(GameEngine2D::Geometry::Vector2D<int>{ dx, 0 });
}
