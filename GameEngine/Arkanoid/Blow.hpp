#pragma once

#include <Animation.hpp>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Missile.hpp"


class Blow final //: public GameEngine::Abstract::Collidable
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int   COLLISION_HALF_WIDTH { 20 };
    static constexpr int   COLLISION_HALF_HEIGHT{ 20 };
    static constexpr float DURATION{ 5.f };

public:

    Blow(Vec2i const& pos, std::shared_ptr<std::vector<GameEngine::Surface> const> const& frames, GameEngine::Colour chroma);
    Blow(Blow const&) noexcept = default;
    Blow(Blow&&)      noexcept = default;

    Blow& operator=(Blow const&) noexcept = default;
    Blow& operator=(Blow&&)      noexcept = default;
    
    ~Blow() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, Rec2i const& clip) const;
    void update(float dt);

    bool is_ended() const noexcept;

    void throw_ball(Ball& ball) const;

private:

    static constexpr float BALL_ACCELERATION{ 10.0f };

    Vec2i pos;

    std::pair<GameEngine::Animation, GameEngine::Colour> blow_effect;
};