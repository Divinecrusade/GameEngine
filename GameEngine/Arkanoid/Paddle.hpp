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
    Paddle(GameEngine2D::Geometry::Vector2D<int> init_pos, float init_speed, int init_half_width);
    Paddle(Paddle const&) = delete;
    Paddle(Paddle&&) = delete;

    Paddle& operator=(Paddle const&) = delete;
    Paddle& operator=(Paddle&&) = delete;

    void draw(GameEngine2D::Interfaces::IGraphics2D& gfx);
    void update(float dt);
    void set_direction(Direction dir);
    void accelerate(float a);

private:

    static constexpr GameEngine2D::Colour MAIN_COLOUR { GameEngine2D::Colours::WHITE };
    static constexpr int HALF_HEIGHT{ 10 };
    
    GameEngine2D::Geometry::Vector2D<int> cur_pos;
    Direction cur_dir{ Direction::STOP };
    float cur_speed;
    int cur_half_width;
};