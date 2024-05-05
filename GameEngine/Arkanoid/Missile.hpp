#pragma once

#include <Animation.hpp>

#include "PlayField.hpp"
#include <Rectangle2D.hpp>


class Missile final
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int COLLISION_HALF_WIDTH { 12 };
    static constexpr int COLLISION_HALF_HEIGHT{ 21 };

public:

    Missile(Vec2i const& init_pos, float init_speed, GameEngine::Surface const& sprite, GameEngine::Colour chroma);
    Missile(Missile const& other) noexcept;
    Missile(Missile&& other_tmp)  noexcept; 

    Missile& operator=(Missile const&) = delete;
    Missile& operator=(Missile&&) = delete;

    ~Missile() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, GameEngine::Geometry::Rectangle2D<int> const& clip) const;
    void update(float dt);

    Vec2i get_pos() const noexcept;

    bool is_collided_with(PlayField const& border) const noexcept;
    bool is_collided_with(Ball const& ball)        const noexcept;
    bool is_collided_with(Paddle const& padd)      const noexcept;

    Rec2i get_collision_box() const;

private:

    void swap(Missile&& other_tmp) noexcept;

private:

    static constexpr Vec2f DIR{ 0.f, 1.f };

    bool destroyed{ false };

    Vec2i cur_pos;

    float cur_speed;
    Vec2f cur_vel;

    GameEngine::Surface sprite;
    GameEngine::Colour const chroma;
};