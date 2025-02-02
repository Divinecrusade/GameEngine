#pragma once

#include <KeyColor.hpp>
#include <IModel.hpp>
#include <Transformations2D.hpp>

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

    Star() = delete;
    Star(Star const&) = default;
    Star(Star &&)     = default;
    Star(Vec2f const& init_pos, float init_outer_radius, int init_flares_count, GameEngine::Colour init_border_colour, float init_rotation_speed) noexcept;

    Star& operator=(Star const&) = delete;
    Star& operator=(Star &&)     = delete;

    ~Star() = default;

    GameEngine::Shape get_shape()   const override;
    GameEngine::Colour get_colour() const noexcept;

    void update(float dt);

private:

    Vec2f const pos;
    float const outer_radius;
    int const flares_count;
    GameEngine::Colour const border_colour;

    float const rotation_speed;

    float cur_rotation_angle{ std::numbers::pi / 2.f };
};