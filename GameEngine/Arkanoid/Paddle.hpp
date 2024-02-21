#pragma once

#include "../Rectangle2D.hpp"
#include "../IGraphics2D.hpp"

#include "Ball.hpp"
#include "../Auxiliry.hpp"


using namespace GameEngine::Geometry::Literals;


class Paddle final
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    enum class Direction
    {
        LEFT = -1, RIGHT = 1, STOP = 0
    };

    Paddle() = delete;
    Paddle(Vec2i init_pos, float init_speed, int init_half_width) noexcept;
    Paddle(Paddle const&) = delete;
    Paddle(Paddle&&)      = delete;

    Paddle& operator=(Paddle const&) = delete;
    Paddle& operator=(Paddle&&)      = delete;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx);
    void update(float dt) noexcept;

    Direction get_direction() const   noexcept;
    void set_direction(Direction dir) noexcept;

    void move_by(Vec2i const& dpos) noexcept;
    void accelerate(float a)        noexcept;

    bool is_collided_with(Ball const& ball) const noexcept;
    void handle_collision(Ball& ball);

    Rec2i get_collision_box() const noexcept;

    bool is_cooldowned() const noexcept;
    void reset_cooldown()      noexcept;

private:

    enum class CollisionEdge
    {
        LEFT, RIGHT, BOTTOM, TOP
    };
    CollisionEdge process(Ball& ball) const noexcept;
    void deflect(Ball& ball, CollisionEdge edge) const;
    inline GameEngine::Geometry::Vector2D<float> calculate_deflect_direction(CollisionEdge edge, double abs_dL, Direction ball_direction) const;

private:

    static constexpr GameEngine::Colour MAIN_COLOUR { GameEngine::Colours::WHITE };
    static constexpr int HALF_HEIGHT{ 6 };
    static constexpr GameEngine::Colour WINGS_COLOUR{ GameEngine::Colours::RED };
    static constexpr int WING_WIDTH{ 5 };

    static constexpr auto   MIN_ANGLE_DEFLECT{ 10._deg };
    static constexpr auto   MAX_ANGLE_DEFLECT{ 80._deg };
    static constexpr auto   MIN_DEFLECT_ZONE_RATIO{ 5._percent };
    
    Vec2i     cur_pos;
    Direction cur_dir{ Direction::STOP };
    float     cur_speed;
    int       cur_half_width;  

    bool cooldown{ false };
};