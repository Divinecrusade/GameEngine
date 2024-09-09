#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <IDrawable.hpp>
#include <array>
#include <random>
#include <algorithm>
#include <ranges>


template<int SIZE, int N_BLOCKS_IN_ROW, GameEngine::Geometry::Vector2D<int> LEFT_TOP_POS>
requires (SIZE > 0 && N_BLOCKS_IN_ROW > 0 && SIZE % N_BLOCKS_IN_ROW == 0)
class ColorField : public GameEngine::Interfaces::IDrawable
{
public:

    static constexpr int BLOCK_SIZE{ SIZE / N_BLOCKS_IN_ROW };

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
            gfx.fill_rectangle(GameEngine::Geometry::Rectangle2D<int>{ LEFT_TOP_POS + BLOCK_SIZE * GameEngine::Geometry::Vector2D<int>{ i % N_BLOCKS_IN_ROW, i / N_BLOCKS_IN_ROW }, BLOCK_SIZE, BLOCK_SIZE }, grid[static_cast<std::size_t>(i)]);
        }
    }

private:

    std::array<GameEngine::Colour, N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW> grid;
};