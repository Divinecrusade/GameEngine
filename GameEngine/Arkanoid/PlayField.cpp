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
    gfx.draw_line(GameEngine2D::Geometry::Vector2D{ collision_frame.left + inner_border_thickness, 
                                                    collision_frame.bottom - inner_border_thickness },
                  GameEngine2D::Geometry::Vector2D{ collision_frame.right - inner_border_thickness,
                                                    collision_frame.bottom - inner_border_thickness }, lose_zone_line_thickness, lose_zone_colour);
}
