#include "Score.hpp"


Score::Score(GameEngine::Geometry::Vector2D<int> const& left_top_pos)
:
left_top_pos{ left_top_pos }
{ }

void Score::brick_destroyed(int cur_n_lives)
{
    points += (FLAT_POINTS_FOR_BRICK + FLAT_POINTS_FOR_EACH_LIFE * cur_n_lives) * cur_points_factor;
    cur_points_factor += DELTA_POINTS_FACTOR;
}

void Score::ball_reflected_by_paddle()
{
    cur_points_factor = MIN_POINTS_FACTOR;
}

void Score::ball_missed()
{
    cur_points_factor = MIN_POINTS_FACTOR;
}

void Score::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(clipping_area.has_value());

    gfx.draw_text(L"Score:", POINTS_FONT_COLOUR, FONT, POINTS_FONT_SIZE * 5 / 4, POINTS_FONT_WEIGHT * 5 / 4, *clipping_area);
    gfx.draw_text(std::to_wstring(points), POINTS_FONT_COLOUR, FONT, POINTS_FONT_SIZE, POINTS_FONT_WEIGHT, GameEngine::Geometry::Rectangle2D<int>{ clipping_area->left, clipping_area->right, clipping_area->top + 75, clipping_area->bottom });

    std::wstring factor{ L"x" };
    factor += std::to_wstring(cur_points_factor);

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

    gfx.draw_text(factor, c, FONT, font_size, POINTS_FONT_WEIGHT, GameEngine::Geometry::Rectangle2D<int>{ clipping_area->left, clipping_area->right, clipping_area->top + 150, clipping_area->bottom });
}