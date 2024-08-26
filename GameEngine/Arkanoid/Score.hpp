#pragma once

#include <IDrawable.hpp>


class Score final : public GameEngine::Interfaces::IDrawable
{
public:

    Score() = delete;
    Score(GameEngine::Geometry::Vector2D<int> const& left_top_pos, GameEngine::Colour font_colour) noexcept;
    Score(Score const&) = delete;
    Score(Score&&)      = delete;

    Score& operator=(Score const&) = delete;
    Score& operator=(Score&&)      = delete;

    ~Score() noexcept = default;

    void brick_destroyed(int cur_n_lives) noexcept;
    void ball_reflected_by_paddle() noexcept;
    void ball_missed() noexcept;

    int get_points() const noexcept;

    void reset() noexcept;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override;

private:

    static constexpr int FLAT_POINTS_FOR_BRICK{ 1 };
    static constexpr int FLAT_POINTS_FOR_EACH_LIFE{ 1 };
    static constexpr int MIN_POINTS_FACTOR{ 1 };
    static constexpr int DELTA_POINTS_FACTOR{ 1 };

    static constexpr GameEngine::DWriteFontNames  FONT{ GameEngine::DWriteFontNames::VERDANA };
    static constexpr int                          POINTS_FONT_SIZE{ 42 };
    static constexpr int                          POINTS_FONT_WEIGHT{ 600 };

    static constexpr std::array<GameEngine::Colour, 4U> FACTORS_COLORS{ GameEngine::Colours::CORAL, GameEngine::Colours::CRIMSON, GameEngine::Colours::ROYAL_BLUE, GameEngine::Colours::GOLD };
    static constexpr std::array<int, 4U>                FACTORS_FONT_SIZES{{ 42, 46, 48, 50 }};

    
    GameEngine::Colour const POINTS_FONT_COLOUR;

    GameEngine::Geometry::Vector2D<int> left_top_pos;

    int cur_points_factor{ MIN_POINTS_FACTOR };
    int points{ 0 };
};