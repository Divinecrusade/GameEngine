#pragma once

#include <Rectangle2D.hpp>
#include <KeyColor.hpp>
#include <IGraphics2D.hpp>
#include <Collidable.hpp>

#include "Ball.hpp"


class Brick final : public GameEngine::Abstract::Collidable
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int WIDTH { 40 };
    static constexpr int HEIGHT{ 20 };

    static constexpr Vec2i              DEFAULT_POS{ 0, 0 };
    static constexpr GameEngine::Colour DEFAULT_COLOUR{ GameEngine::Colours::MAGENTA };

    Brick() noexcept;
    Brick(Vec2i const& pos, GameEngine::Colour c) noexcept;
    Brick(Brick const& b) noexcept;
    Brick(Brick&& b)      noexcept;
    Brick& operator=(Brick const& b) noexcept;
    Brick& operator=(Brick&& b)      noexcept;

    ~Brick() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx) const;

    Rec2i get_collision_box() const override;

    void deflect(Ball& ball) const noexcept;

private:

    void swap(Brick&& b) noexcept;
    friend bool operator==(Brick const& lhs, Brick const& rhs) noexcept;

private:

    static constexpr float LEFT_FACTOR_C { 1.10f };
    static constexpr float RIGHT_FACTOR_C{ 0.65f };
    static constexpr float BOT_FACTOR_C  { 0.80f };
    static constexpr float TOP_FACTOR_C  { 0.93f };

    static constexpr int FRAME_WIDTH{ 3 };

    static constexpr int VISUAL_PADDING{ 1 };

    Rec2i collision_box;
    GameEngine::Colour c;
};

bool operator==(Brick const& lhs, Brick const& rhs) noexcept;
bool operator!=(Brick const& lhs, Brick const& rhs) noexcept;