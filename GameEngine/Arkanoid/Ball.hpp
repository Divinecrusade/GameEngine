#pragma once

#include "../Rectangle2D.hpp"
#include "../KeyColor.hpp"
#include "../IGraphics2D.hpp"


class Ball final
{
public:

    Ball() noexcept = delete;
    Ball(GameEngine::Geometry::Vector2D<int> init_pos, GameEngine::Geometry::Vector2D<float> init_dir, float init_speed) noexcept;
    Ball(Ball const&) = delete;
    Ball(Ball&&) = delete;

    Ball& operator=(Ball const&) = delete;
    Ball& operator=(Ball&&) = delete;

    ~Ball() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;
    void update(float dt) noexcept;
    
    void accelerate(float a) noexcept;

    GameEngine::Geometry::Rectangle2D<int> get_collision_box() const noexcept;
    GameEngine::Geometry::Vector2D<int> get_center() const noexcept;
    GameEngine::Geometry::Vector2D<float> get_velocity() const noexcept;

    void change_direction(GameEngine::Geometry::Vector2D<float> const& dir);

    void move_by(GameEngine::Geometry::Vector2D<int> const& dpos) noexcept;

    void inverse_x() noexcept;
    void inverse_y() noexcept;

private:

    static constexpr GameEngine::Colour c{ GameEngine::Colours::LIGHT_GOLDEN_ROD_YELLOW };
    static constexpr int RADIUS{ 8 };

    GameEngine::Geometry::Vector2D<int> cur_pos;
    float cur_speed;

    GameEngine::Geometry::Vector2D<float> cur_vel;
};