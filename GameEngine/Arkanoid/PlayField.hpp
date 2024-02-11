#pragma once

#include "../Rectangle2D.hpp"
#include "../IGraphics2D.hpp"

#include "Paddle.hpp"

class PlayField final
{
public:

    PlayField() = delete;
    PlayField(GameEngine::Geometry::Rectangle2D<int> const& size_and_location) noexcept;
    PlayField(PlayField const&) = delete;
    PlayField(PlayField&&) = delete;

    PlayField& operator=(PlayField const&) = delete;
    PlayField& operator=(PlayField&&) = delete;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;
    
    bool is_in_field(Paddle const& pad) const noexcept;
    void handle_collision(Paddle& pad) const noexcept;

    bool is_in_field(Ball const& ball) const noexcept;
    bool is_in_lose_zone(Ball const& ball) const noexcept;
    void handle_collision(Ball& ball) const noexcept;

    GameEngine::Geometry::Rectangle2D<int> get_collision_box() const noexcept;

private:

    static constexpr GameEngine::Colour inner_border_colour{ GameEngine::Colours::ROYAL_BLUE };
    static constexpr int inner_border_thickness{ 2 };
    static constexpr GameEngine::Colour outer_border_colour{ GameEngine::Colours::CYAN };
    static constexpr int outer_border_thickness{ 2 };
    static constexpr GameEngine::Colour lose_zone_colour{ GameEngine::Colours::RED };
    static constexpr int lose_zone_line_thickness{ 2 };

    GameEngine::Geometry::Rectangle2D<int> const collision_frame;
};