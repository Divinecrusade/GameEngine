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

void Score::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const
{
    assert(clipping_area.has_value());
    gfx.draw_text(std::to_wstring(points), POINTS_FONT_COLOUR, FONT, POINTS_FONT_SIZE, POINTS_FONT_WEIGHT, *clipping_area);
}