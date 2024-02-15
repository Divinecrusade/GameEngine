#pragma once

#include "../Animation.hpp"

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Missile.hpp"


class Blow
{
public:

    static constexpr int COLLISION_HALF_WIDTH{ 20 };
    static constexpr int COLLISION_HALF_HEIGHT{ 20 };

public:

    Blow(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Animation const& blow_effect, GameEngine::Colour chroma);
    Blow(Blow const& other) noexcept;
    Blow(Blow&& other_tmp) noexcept;

    Blow& operator=(Blow const&) = delete;
    Blow& operator=(Blow&&) = delete;
    
    ~Blow() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, GameEngine::Geometry::Rectangle2D<int> const& clip) const;
    void update(float dt);

    bool is_ended() const noexcept;

    bool is_collided_with(Ball const& ball) const;
    bool is_collided_with(Paddle const& padd) const;
    bool is_collided_with(Missile const& missile) const;

    void throw_ball(Ball& ball) const;

private:

    static constexpr float FRAME_DURATION{ 0.25f };
    static constexpr float BALL_ACCELERATION{ 20.f };

    GameEngine::Geometry::Vector2D<int> const pos;

    GameEngine::Animation blow_effect;
    GameEngine::Colour const chroma;
};