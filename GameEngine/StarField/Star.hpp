#pragma once

#include <KeyColor.hpp>
#include <IModel.hpp>
#include <Transformations2D.hpp>
#include <Collidable.hpp>

#include <numbers>
#include <cmath>


class Star : public GameEngine::Interfaces::IModel
{
public:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2f = GameEngine::Geometry::Rectangle2D<float>;

public:

    static constexpr int STROKE_WIDTH{ 2 };

private:

    static constexpr float OUTER_RADIUS_MODIFIER{ 2.f };
    
public:

    Star() = delete;
    Star(Star const&) = default;
    Star(Star &&)     = default;
    Star(Vec2f const& init_pos, float init_outer_radius, int init_flares_count, 
    GameEngine::Colour init_border_colour, float init_rotation_speed, 
    float init_min_size_factor, float init_colour_freq_factor, float init_colour_phase) noexcept;

    Star& operator=(Star const&) = delete;
    Star& operator=(Star &&)     = delete;

    virtual ~Star() = default;

    Rec2f get_square() const;

    GameEngine::Shape get_shape()   const override;
    GameEngine::Colour get_colour() const noexcept;

    void update(float dt);

private:

    Vec2f const pos;
    float const outer_radius;
    int   const flares_count;
    GameEngine::Colour base_colour;
    GameEngine::Colour cur_colour;

    float const rotation_speed;
    float const min_size_factor;
    float const colour_freq_factor;
    float const colour_phase;
    float delta_size;

    float time{ 0.f };
    float cur_rotation_angle{ std::numbers::pi / 2.f };
    float cur_size_factor{ 1.f };
};