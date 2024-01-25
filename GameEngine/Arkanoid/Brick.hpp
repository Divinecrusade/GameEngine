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
    static constexpr GameEngine::Colour DEFAULT_COLOUR{ GameEngine::Colours::MAGENTA };

    Brick();
    Brick(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Colour c);
    Brick(Brick const& b);
    Brick(Brick&& b) noexcept;
    Brick& operator=(Brick const& b);
    Brick& operator=(Brick&& b) noexcept;

    ~Brick() = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;

private:

    void swap(Brick&& b) noexcept;

private:

    static constexpr int VISUAL_PADDING{ 1 };

    GameEngine::Geometry::Rectangle2D<int> collision_box;
    GameEngine::Colour c;
};