#pragma once

#include <Rectangle2D.hpp>
#include <KeyColor.hpp>
#include <IGraphics2D.hpp>
#include <IDrawable.hpp>
#include <Collidable.hpp>

#include "Ball.hpp"


class Brick final : public GameEngine::Abstract::Collidable, public GameEngine::Interfaces::IDrawable
{
private:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int WIDTH { 40 };
    static constexpr int HEIGHT{ 20 };

public:

    Brick() noexcept;
    Brick(Vec2i const& pos, GameEngine::Colour c) noexcept;
    Brick(Brick const&) noexcept = default;
    Brick(Brick&&)      noexcept = default;
    Brick& operator=(Brick const& other) noexcept
    { 
        assert(this != &other);

        collision_box = other.collision_box;
        c = other.c;

        return *this;
    }
    Brick& operator=(Brick&& other_tmp) noexcept
    {
        assert(this != &other_tmp);

        std::swap(collision_box, other_tmp.collision_box);
        std::swap(c, other_tmp.c);

        return *this;
    }

    ~Brick() noexcept = default;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override;

    Rec2i get_collision_box() const noexcept override;

    void deflect(Ball& ball) const noexcept;

private:

private:

    static constexpr float LEFT_FACTOR_C { 1.10f };
    static constexpr float RIGHT_FACTOR_C{ 0.65f };
    static constexpr float BOT_FACTOR_C  { 0.80f };
    static constexpr float TOP_FACTOR_C  { 0.93f };

    static constexpr Vec2i              DEFAULT_POS{ 0, 0 };
    static constexpr GameEngine::Colour DEFAULT_COLOUR{ GameEngine::Colours::MAGENTA };

    static constexpr int FRAME_WIDTH{ 3 };

    static constexpr int VISUAL_PADDING{ 1 };

    Rec2i collision_box;
    GameEngine::Colour c;
};