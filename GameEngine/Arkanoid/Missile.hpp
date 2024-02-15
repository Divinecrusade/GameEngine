#pragma once

#include "../Animation.hpp"

#include "PlayField.hpp"
#include "../Rectangle2D.hpp"


class Missile
{
public:

    static constexpr int COLLISION_HALF_WIDTH{ 12 };
    static constexpr int COLLISION_HALF_HEIGHT{ 21 };

public:

    Missile(GameEngine::Geometry::Vector2D<int> const& init_pos, float init_speed, GameEngine::Surface const& sprite, GameEngine::Colour chroma);
    Missile(Missile const& other) noexcept;
    Missile(Missile&& other_tmp) noexcept; 

    ~Missile() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, GameEngine::Geometry::Rectangle2D<int> const& clip);
    void update(float dt);

    GameEngine::Geometry::Vector2D<int> get_pos() const noexcept;

    bool is_collided_with(PlayField const& border) const;
    bool is_collided_with(Ball const& ball) const;
    bool is_collided_with(Paddle const& padd) const;

    GameEngine::Geometry::Rectangle2D<int> get_collision_box() const;

private:

    void swap(Missile&& other_tmp) noexcept;

private:

    static constexpr GameEngine::Geometry::Vector2D<float> DIR{ 0.f, 1.f };

    bool destroyed{ false };

    GameEngine::Geometry::Vector2D<int>   cur_pos;

    float cur_speed;
    GameEngine::Geometry::Vector2D<float> cur_vel;

    GameEngine::Surface sprite;
    GameEngine::Colour const chroma;
};