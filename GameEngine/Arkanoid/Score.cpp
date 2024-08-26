#include "Score.hpp"


Score::Score(GameEngine::Geometry::Vector2D<int> const& left_top_pos, GameEngine::Colour font_colour) noexcept
:
POINTS_FONT_COLOUR{ font_colour },
left_top_pos{ left_top_pos }
{ }

void Score::brick_destroyed(int cur_n_lives) noexcept
{
    assert(cur_n_lives >= 0);

    points += (FLAT_POINTS_FOR_BRICK + FLAT_POINTS_FOR_EACH_LIFE * cur_n_lives) * cur_points_factor;
    cur_points_factor += DELTA_POINTS_FACTOR;
}

void Score::ball_reflected_by_paddle() noexcept
{
    cur_points_factor = MIN_POINTS_FACTOR;
}

void Score::ball_missed() noexcept
{
    cur_points_factor = MIN_POINTS_FACTOR;
}

int Score::get_points() const noexcept
{
    return points;
}

void Score::reset() noexcept
{
    cur_points_factor = MIN_POINTS_FACTOR;
    points = 0;
}

void Score::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(clipping_area.has_value());

    gfx.draw_text(L"Score:", POINTS_FONT_COLOUR, FONT, POINTS_FONT_SIZE * 5 / 4, POINTS_FONT_WEIGHT * 5 / 4, *clipping_area);
    gfx.draw_text(std::to_wstring(points), POINTS_FONT_COLOUR, FONT, POINTS_FONT_SIZE, POINTS_FONT_WEIGHT, GameEngine::Geometry::Rectangle2D<int>{ clipping_area->left, clipping_area->right, clipping_area->bottom, clipping_area->top + 75 });

    std::wstring const factor{ L"x" + std::to_wstring(cur_points_factor) };

    GameEngine::Colour c{ FACTORS_COLORS[3U] };
    int font_size{ FACTORS_FONT_SIZES[3U] };

    if (cur_points_factor <= 5)
    {
        c = FACTORS_COLORS[0U];
        font_size = FACTORS_FONT_SIZES[0U];
    }
    else if (cur_points_factor <= 10)
    {
        c = FACTORS_COLORS[1U];
        font_size = FACTORS_FONT_SIZES[1U];
    }
    else if (cur_points_factor <= 15)
    {
        c = FACTORS_COLORS[2U];
        font_size = FACTORS_FONT_SIZES[2U];
    }

    gfx.draw_text(factor, c, FONT, font_size, POINTS_FONT_WEIGHT, GameEngine::Geometry::Rectangle2D<int>{ clipping_area->left, clipping_area->right, clipping_area->bottom, clipping_area->top + 150 });
}