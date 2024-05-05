#pragma once

#include <Rectangle2D.hpp>
#include <KeyColor.hpp>
#include <IGraphics2D.hpp>
#include <Collidable.hpp>


class Ball final : public GameEngine::Abstract::Collidable
{
private:

    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;

public:

    static constexpr float MIN_ABS_X_VEL{ 60.f };
    static constexpr float MIN_ABS_Y_VEL{ 60.f };

    static constexpr int RADIUS{ 8 };

public:

    Ball() noexcept = delete;
    Ball(Vec2i init_pos, GameEngine::Geometry::Vector2D<float> init_dir, float init_speed) noexcept;
    Ball(Ball const&) = delete;
    Ball(Ball&&)      = delete;

    Ball& operator=(Ball const&) = delete;
    Ball& operator=(Ball&&)      = delete;

    ~Ball() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;
    void update(float dt) noexcept;
    
    void accelerate(float a) noexcept;

    Rec2i get_collision_box() const noexcept override;
    
    GameEngine::Geometry::Vector2D<float> get_velocity() const noexcept;

    void change_direction(GameEngine::Geometry::Vector2D<float> const& dir);

    void move_by(Vec2i const& dpos) noexcept;

    void inverse_x() noexcept;
    void inverse_y() noexcept;

private:

    static constexpr GameEngine::Colour c{ GameEngine::Colours::LIGHT_GOLDEN_ROD_YELLOW };

    Vec2i cur_pos;
    float cur_speed;

    GameEngine::Geometry::Vector2D<float> cur_vel;
};