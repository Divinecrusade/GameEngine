#pragma once

#include "../Rectangle2D.hpp"
#include "../KeyColor.hpp"
#include "../IGraphics2D.hpp"


class Brick final
{
public:

    static constexpr int WIDTH{ 60 };
    static constexpr int HEIGHT{ 30 };

    static constexpr int DEFAULT_POS_X{ 0 };
    static constexpr int DEFAULT_POS_Y{ 0 };
    static constexpr GameEngine2D::Colour DEFAULT_COLOUR{ GameEngine2D::Colours::MAGENTA };

    Brick();
    Brick(GameEngine2D::Geometry::Vector2D<int> const& pos, GameEngine2D::Colour c);
    Brick(Brick const& b);
    Brick(Brick&& b) noexcept;
    Brick& operator=(Brick const& b);
    Brick& operator=(Brick&& b) noexcept;

    ~Brick() = default;

    void draw(GameEngine2D::Interfaces::IGraphics2D& gfx) const;

private:

    void swap(Brick&& b) noexcept;

private:

    static constexpr int VISUAL_PADDING{ 1 };

    GameEngine2D::Geometry::Rectangle2D<int> collision_box;
    GameEngine2D::Colour c;
};