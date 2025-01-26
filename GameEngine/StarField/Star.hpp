#pragma once

#include <KeyColor.hpp>
#include <IModel.hpp>

#include <numbers>
#include <cmath>


class Star : GameEngine::Interfaces::IModel
{
public:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;

public:

    static constexpr int STROKE_WIDTH{ 2 };

private:

    static constexpr float OUTER_RADIUS_MODIFIER{ 2.f };
    
public:

    Star(Vec2f const& init_pos, float init_outer_radius, int init_flares_count, GameEngine::Colour init_border_colour) noexcept;

    GameEngine::Shape get_shape() const override;
    GameEngine::Colour get_colour() const noexcept;

private:

    Vec2f pos{ };
    float outer_radius{ };
    int flares_count{ };
    GameEngine::Colour border_colour{ };
};