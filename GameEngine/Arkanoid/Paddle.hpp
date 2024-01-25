#pragma once

#include "../Rectangle2D.hpp"
#include "../IGraphics2D.hpp"


class Paddle final
{
public:

    enum class Direction
    {
        LEFT = -1, RIGHT = 1, STOP = 0
    };

    Paddle() = delete;
    Paddle(GameEngine::Geometry::Vector2D<int> init_pos, float init_speed, int init_half_width);
    Paddle(Paddle const&) = delete;
    Paddle(Paddle&&) = delete;

    Paddle& operator=(Paddle const&) = delete;
    Paddle& operator=(Paddle&&) = delete;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx);
    void update(float dt);
    Direction get_direction() const noexcept;
    void set_direction(Direction dir);
    void accelerate(float a);
    GameEngine::Geometry::Rectangle2D<int> get_collision() const;
    void move_by(GameEngine::Geometry::Vector2D<int> const& dpos) noexcept;

private:

    static constexpr GameEngine::Colour MAIN_COLOUR { GameEngine::Colours::WHITE };
    static constexpr int HALF_HEIGHT{ 10 };
    static constexpr GameEngine::Colour WINGS_COLOUR{ GameEngine::Colours::RED };
    static constexpr int WING_WIDTH{ 5 };
    
    GameEngine::Geometry::Vector2D<int> cur_pos;
    Direction cur_dir{ Direction::STOP };
    float cur_speed;
    int cur_half_width;
};