#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <IDrawable.hpp>
#include <array>
#include <random>
#include <algorithm>
#include <ranges>


template<int size, int n_blocks_in_row, GameEngine::Geometry::Vector2D<int> pos>
requires (size > 0 && n_blocks_in_row > 0 && size % n_blocks_in_row == 0)
class ColorField : public GameEngine::Interfaces::IDrawable
{
public:

    ColorField() = delete;

    template<std::size_t N>
    ColorField(std::array<GameEngine::Colour, N> const& colors_pull)
    {
        std::ranges::generate(grid, 
        [rng{ std::mt19937{ std::random_device{}() } }, color_distr{ std::uniform_int_distribution<std::size_t>{ 0U, N - 1U } }, &colors_pull]
        () mutable
        { return colors_pull[color_distr(rng)]; });
    }
    
    ColorField(ColorField const&) = delete;
    ColorField(ColorField&&)      = delete;

    ColorField& operator=(ColorField const&) = delete;
    ColorField& operator=(ColorField&&)      = delete;

    virtual ~ColorField() = default;


    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        for (int i{ 0 }; i != static_cast<int>(grid.size()); ++i)
        {
            gfx.fill_rectangle(GameEngine::Geometry::Rectangle2D<int>{ LEFT_TOP_POS + BLOCK_SIZE * GameEngine::Geometry::Vector2D<int>{ i % n_blocks_in_row, i / n_blocks_in_row }, BLOCK_SIZE, BLOCK_SIZE }, grid[static_cast<std::size_t>(i)]);
        }
    }

private:

    static constexpr GameEngine::Geometry::Vector2D<int> LEFT_TOP_POS{ pos };
    static constexpr int BLOCK_SIZE{ size / n_blocks_in_row };

    std::array<GameEngine::Colour, n_blocks_in_row * n_blocks_in_row> grid;
};