#pragma once

#include <Animation.hpp>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Missile.hpp"


class Blow final : public GameEngine::Abstract::Collidable
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int COLLISION_HALF_WIDTH { 20 };
    static constexpr int COLLISION_HALF_HEIGHT{ 20 };

public:

    Blow(Vec2i const& pos, GameEngine::Animation const& anim, GameEngine::Colour chroma);
    Blow(Blow const&) = default;
    Blow(Blow&&)      = default;

    Blow& operator=(Blow const&) = default;
    Blow& operator=(Blow&&)      = default;
    
    ~Blow() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, Rec2i const& clip) const;
    void update(float dt);

    bool is_ended() const noexcept;

    void throw_ball(Ball& ball) const;

    Rec2i get_collision_box() const override;

private:

    static constexpr float BALL_ACCELERATION{ 10.0f };

    Vec2i pos;

    std::pair<GameEngine::Animation, GameEngine::Colour> blow_effect;
};