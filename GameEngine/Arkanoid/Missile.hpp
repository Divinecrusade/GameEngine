#pragma once

#include <Animation.hpp>
#include <SurfaceView.hpp>
#include <Rectangle2D.hpp>
#include <Collidable.hpp>
#include <IDrawable.hpp>
#include "PlayField.hpp"


class Missile final : public GameEngine::Abstract::Collidable, public GameEngine::Interfaces::IDrawable
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int COLLISION_HALF_WIDTH { 12 };
    static constexpr int COLLISION_HALF_HEIGHT{ 21 };

public:

    Missile() = delete;
    Missile(Vec2i const& init_pos, float init_speed, GameEngine::SurfaceView sprite, GameEngine::Colour chroma);
    Missile(Missile const&) = default;
    Missile(Missile&&)      = default;

    Missile& operator=(Missile const& other_missile) noexcept;
    Missile& operator=(Missile&& other_missile_tmp)  noexcept;

    ~Missile() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override;
    void update(float dt);

    Rec2i get_collision_box() const noexcept override;
    Vec2i get_pos() const noexcept;

    void destroy() noexcept;
    bool is_destroyed() const noexcept;

private:

    static constexpr Vec2f DIR{ 0.f, 1.f };

    bool  destroyed{ false };

    Vec2i cur_pos;

    float cur_speed;
    Vec2f cur_vel;

    std::pair<GameEngine::SurfaceView, GameEngine::Colour> sprite;
};