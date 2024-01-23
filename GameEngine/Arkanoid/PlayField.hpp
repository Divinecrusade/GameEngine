#pragma once

#include "../Rectangle2D.hpp"
#include "../IGraphics2D.hpp"


class PlayField final
{
public:

    PlayField() = delete;
    PlayField(GameEngine2D::Geometry::Rectangle2D<int> const& size_and_location);
    PlayField(PlayField const&) = delete;
    PlayField(PlayField&&) = delete;

    PlayField& operator=(PlayField const&) = delete;
    PlayField& operator=(PlayField&&) = delete;

    void draw(GameEngine2D::Interfaces::IGraphics2D& gfx) const;
    
    //bool is_in_field(Paddle pad) const;
    //bool is_in_field(Ball ball) const;

private:

    static constexpr GameEngine2D::Colour inner_border_colour{ GameEngine2D::Colours::BLUE };
    static constexpr int inner_border_thickness{ 2 };
    static constexpr GameEngine2D::Colour outer_border_colour{ GameEngine2D::Colours::CYAN };
    static constexpr int outer_border_thickness{ 2 };

    GameEngine2D::Geometry::Rectangle2D<unsigned> const collision_frame;
};