#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <IDrawable.hpp>
#include <ContiguousIterator.hpp>

#include <array>
#include <random>
#include <algorithm>
#include <ranges>


template<int SIZE, int N_BLOCKS_IN_ROW, GameEngine::Geometry::Vector2D<int> LEFT_TOP_POS>
requires (SIZE > 0 && N_BLOCKS_IN_ROW > 0 && SIZE % N_BLOCKS_IN_ROW == 0)
class ColourField : public GameEngine::Interfaces::IDrawable
{
public:

    static constexpr int BLOCK_SIZE{ SIZE / N_BLOCKS_IN_ROW };

    using iterator = GameEngine::Auxiliary::ContiguousIterator<GameEngine::Colour, ColourField>;

public:

    ColourField() = delete;

    template<std::size_t N>
    ColourField(std::array<GameEngine::Colour, N> const& colors_pull)
    {
        std::ranges::generate(grid, 
        [rng{ std::mt19937{ std::random_device{}() } }, color_distr{ std::uniform_int_distribution<std::size_t>{ 0U, N - 1U } }, &colors_pull]
        () mutable
        { return colors_pull[color_distr(rng)]; });
    }
    
    ColourField(ColourField const&) = delete;
    ColourField(ColourField&&)      = delete;

    ColourField& operator=(ColourField const&) = delete;
    ColourField& operator=(ColourField&&)      = delete;

    virtual ~ColourField() = default;


    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        for (int i{ 0 }; i != static_cast<int>(grid.size()); ++i)
        {
            gfx.fill_rectangle(GameEngine::Geometry::Rectangle2D<int>{ LEFT_TOP_POS + BLOCK_SIZE * GameEngine::Geometry::Vector2D<int>{ i % N_BLOCKS_IN_ROW, i / N_BLOCKS_IN_ROW }, BLOCK_SIZE, BLOCK_SIZE }, grid[static_cast<std::size_t>(i)]);
        }
    }

    iterator get_block(GameEngine::Geometry::Vector2D<int> coordinate)
    {
        coordinate -= LEFT_TOP_POS;
        coordinate /= BLOCK_SIZE;

        return iterator{ grid.data() + coordinate.y * N_BLOCKS_IN_ROW + coordinate.x };
    }

private:

    std::array<GameEngine::Colour, N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW> grid;
};