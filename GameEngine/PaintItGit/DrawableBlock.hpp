#pragma once

#include "ColourBlock.hpp"


template<int SIZE>
requires (SIZE > 0)
class DrawableBlock : public ColourBlock, public GameEngine::Interfaces::IDrawable
{
public:

    static constexpr GameEngine::Geometry::Vector2D<int> DEFAULT_LEFT_TOP_POS{ 0, 0 };

public:

    constexpr DrawableBlock(GameEngine::Geometry::Vector2D<int> const& left_top_pos = DEFAULT_LEFT_TOP_POS, GameEngine::Colour c = ColourBlock::DEFAULT_COLOUR) noexcept
    :
    ColourBlock{ c },
    collision_box{ left_top_pos, SIZE, SIZE }
    { }
    constexpr DrawableBlock(DrawableBlock const&) noexcept = default;
    constexpr DrawableBlock(DrawableBlock&&) noexcept      = default;

    constexpr DrawableBlock& operator=(DrawableBlock const&) = default;
    constexpr DrawableBlock& operator=(DrawableBlock&&)      = default;

    virtual constexpr ~DrawableBlock() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        gfx.fill_rectangle(collision_box, this->get_colour());
    }

protected:

    constexpr GameEngine::Geometry::Rectangle2D<int> const& get_collision_box() const noexcept
    {
        return collision_box;
    }

private:

    GameEngine::Geometry::Rectangle2D<int> collision_box;
};