#pragma once

#include "../Rectangle2D.hpp"
#include "../KeyColor.hpp"
#include "../IGraphics2D.hpp"


class Ball final
{
public:

    Ball() = delete;
    Ball(GameEngine::Geometry::Vector2D<int> init_pos, GameEngine::Geometry::Vector2D<float> init_vel);
    Ball(Ball const&) = delete;
    Ball(Ball&&) = delete;

    Ball& operator=(Ball const&) = delete;
    Ball& operator=(Ball&&) = delete;

    ~Ball() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;
    void update(float dt);
    
    void accelerate(float a);

    GameEngine::Geometry::Rectangle2D<int> get_collision_box() const;
    GameEngine::Geometry::Vector2D<float> get_velocity() const;

    void move_by(GameEngine::Geometry::Vector2D<int> const& dpos);

    void inverse_x();
    void inverse_y();

private:

    static constexpr GameEngine::Colour c{ GameEngine::Colours::LIGHT_GOLDEN_ROD_YELLOW };
    static constexpr int RADIUS{ 10 };

    GameEngine::Geometry::Vector2D<int> cur_pos;
    GameEngine::Geometry::Vector2D<float> cur_vel;
};