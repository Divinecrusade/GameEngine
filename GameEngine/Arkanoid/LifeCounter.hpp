#pragma once

#include <IDrawable.hpp>
#include <Surface.hpp>


class LifeCounter final : public GameEngine::Interfaces::IDrawable
{
public:
    
    LifeCounter() = delete;
    LifeCounter(int min_value, int max_value, GameEngine::Geometry::Vector2D<int> const& left_top_pos, GameEngine::SurfaceView sprite, GameEngine::Colour chroma = GameEngine::Colours::MAGENTA);
    LifeCounter(LifeCounter const&) = delete;
    LifeCounter(LifeCounter&&) = delete;

    LifeCounter& operator=(LifeCounter const&) = delete;
    LifeCounter& operator=(LifeCounter&&) = delete;
    
    ~LifeCounter() = default;

    bool is_ended() const noexcept;
    
    int get_cur_n_lives() const noexcept;

    void reset() noexcept;

    void decrease() noexcept;

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override;

private:

    static constexpr int MARGIN_RIGHT{ 10 };

    int const min_value;
    int const max_value;

    int cur_value;

    GameEngine::Geometry::Vector2D<int> const left_top_pos;
    std::pair<GameEngine::SurfaceView, GameEngine::Colour> const sprite;
};