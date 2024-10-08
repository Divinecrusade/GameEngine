#include "Brick.hpp"


Brick::Brick() noexcept
:
collision_box{ DEFAULT_POS, WIDTH, HEIGHT },
c{ DEFAULT_COLOUR }
{ }

Brick::Brick(Vec2i const& pos, GameEngine::Colour c) noexcept
:
collision_box{ pos, WIDTH, HEIGHT },
c{ c }
{ }

void Brick::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const&) const
{
    auto const padded_block{ collision_box.get_expanded(-VISUAL_PADDING) };
    auto const flat_visual_block{ padded_block.get_expanded(-FRAME_WIDTH) };

    gfx.fill_rectangle(flat_visual_block, c);

    std::vector<Vec2i> points{ 4U };

    points[0U] = Vec2i{ padded_block.left, padded_block.top };
    points[1U] = Vec2i{ padded_block.left, padded_block.bottom };
    points[2U] = points[1U] + Vec2i{ FRAME_WIDTH, -FRAME_WIDTH };
    points[3U] = points[0U] + Vec2i{ FRAME_WIDTH, FRAME_WIDTH  };
    gfx.fill_polygon
    (
        points,
        GameEngine::Colour
        { 
            uint8_t(c[GameEngine::Colour::ComponentIndex::R] * LEFT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::G] * LEFT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::B] * LEFT_FACTOR_C) 
        }
    );

    points[0U] = Vec2i{ padded_block.right, padded_block.top };
    points[1U] = Vec2i{ padded_block.right, padded_block.bottom };
    points[2U] = points[1U] + Vec2i{ -FRAME_WIDTH, -FRAME_WIDTH };
    points[3U] = points[0U] + Vec2i{ -FRAME_WIDTH, FRAME_WIDTH  };
    gfx.fill_polygon
    (
        points, 
        GameEngine::Colour
        { 
            uint8_t(c[GameEngine::Colour::ComponentIndex::R] * RIGHT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::G] * RIGHT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::B] * RIGHT_FACTOR_C) 
        }
    );

    points[0U] = Vec2i{ padded_block.left,  padded_block.bottom };
    points[1U] = Vec2i{ padded_block.right, padded_block.bottom };
    points[2U] = points[1U] + Vec2i{ -FRAME_WIDTH, -FRAME_WIDTH };
    points[3U] = points[0U] + Vec2i{ FRAME_WIDTH, -FRAME_WIDTH  };
    gfx.fill_polygon
    (
        points,
        GameEngine::Colour
        { 
            uint8_t(c[GameEngine::Colour::ComponentIndex::R] * BOT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::G] * BOT_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::B] * BOT_FACTOR_C) 
        }
    );

    points[0U] = Vec2i{ padded_block.left,  padded_block.top };
    points[1U] = Vec2i{ padded_block.right, padded_block.top };
    points[2U] = points[1U] + Vec2i{ -FRAME_WIDTH, FRAME_WIDTH };
    points[3U] = points[0U] + Vec2i{ FRAME_WIDTH, FRAME_WIDTH };
    gfx.fill_polygon
    (
        points,
        GameEngine::Colour
        { 
            uint8_t(c[GameEngine::Colour::ComponentIndex::R] * TOP_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::G] * TOP_FACTOR_C),
            uint8_t(c[GameEngine::Colour::ComponentIndex::B] * TOP_FACTOR_C) 
        }
    );
}

void Brick::deflect(Ball& ball) const noexcept
{
    assert(is_collided_with(ball));

    if (Vec2i const ball_position{ ball.get_collision_box().get_center() }; 
        (ball_position.x >= collision_box.left && ball_position.x <= collision_box.right)
        ||
        (std::signbit(ball.get_direction().x) == GameEngine::Geometry::Auxiliry::signbit((ball_position - collision_box.get_center()).x)))
    {
        ball.inverse_y();
    }
    else
    {
        ball.inverse_x();
    }
}

GameEngine::Geometry::Rectangle2D<int> Brick::get_collision_box() const noexcept
{
    return collision_box;
}