#pragma once

#include <Animation.hpp>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Missile.hpp"


class Blow final
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int COLLISION_HALF_WIDTH { 20 };
    static constexpr int COLLISION_HALF_HEIGHT{ 20 };

public:

    Blow(Vec2i const& pos, GameEngine::Animation const& blow_effect, GameEngine::Colour chroma);
    Blow(Blow const& other) noexcept;
    Blow(Blow&& other_tmp)  noexcept;

    Blow& operator=(Blow const&) = delete;
    Blow& operator=(Blow&&)      = delete;
    
    ~Blow() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, Rec2i const& clip) const;
    void update(float dt);

    bool is_ended() const noexcept;

    bool is_collided_with(Ball const& ball)       const noexcept;
    bool is_collided_with(Paddle const& padd)     const noexcept;
    bool is_collided_with(Missile const& missile) const noexcept;

    void throw_ball(Ball& ball) const;

private:

    static constexpr float FRAME_DURATION   { 0.25f };
    static constexpr float BALL_ACCELERATION{ 20.0f };

    Vec2i const pos;

    GameEngine::Animation blow_effect;
    GameEngine::Colour const chroma;
};