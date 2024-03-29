#pragma once

#include "../Rectangle2D.hpp"
#include "../KeyColor.hpp"
#include "../IGraphics2D.hpp"

#include "Ball.hpp"


class Brick final
{
public:

    static constexpr int WIDTH{ 60 };
    static constexpr int HEIGHT{ 30 };

    static constexpr GameEngine::Geometry::Vector2D<int> DEFAULT_POS{ 0, 0 };
    static constexpr GameEngine::Colour DEFAULT_COLOUR{ GameEngine::Colours::MAGENTA };

    Brick() noexcept;
    Brick(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Colour c) noexcept;
    Brick(Brick const& b) noexcept;
    Brick(Brick&& b) noexcept;
    Brick& operator=(Brick const& b) noexcept;
    Brick& operator=(Brick&& b) noexcept;

    ~Brick() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;

    bool is_colided_with(Ball const& ball) const noexcept;
    void handle_collision(Ball& ball) const noexcept;

    int get_sqr_distance(Ball const& ball) const noexcept;

private:

    void swap(Brick&& b) noexcept;
    friend bool operator==(Brick const& lhs, Brick const& rhs) noexcept;

private:

    static constexpr int VISUAL_PADDING{ 1 };

    GameEngine::Geometry::Rectangle2D<int> collision_box;
    GameEngine::Colour c;
};

bool operator==(Brick const& lhs, Brick const& rhs) noexcept;
bool operator!=(Brick const& lhs, Brick const& rhs) noexcept;