#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <IDrawable.hpp>
#include <ContiguousIterator.hpp>

#include <array>
#include <random>
#include <algorithm>
#include <ranges>
#include <functional>
#include <ranges>


template<int SIZE, int N_BLOCKS_IN_ROW, GameEngine::Geometry::Vector2D<int> LEFT_TOP_POS>
requires (SIZE > 0 && N_BLOCKS_IN_ROW > 0 && SIZE % N_BLOCKS_IN_ROW == 0)
class ColourField final : public GameEngine::Interfaces::IDrawable
{
private:

    using effect = std::optional<std::function<GameEngine::Colour(GameEngine::Colour)>>;
    using block  = std::pair<GameEngine::Colour, effect>;

public:

    static constexpr int BLOCK_SIZE{ SIZE / N_BLOCKS_IN_ROW };

    using iterator = GameEngine::Auxiliary::ContiguousIterator<block, ColourField>;

public:

    ColourField() = delete;

    template<std::size_t N>
    ColourField(std::array<GameEngine::Colour, N> const& colors_pull)
    {
        std::ranges::generate(grid, 
        [rng{ std::mt19937{ std::random_device{}() } }, color_distr{ std::uniform_int_distribution<std::size_t>{ 0U, N - 1U } }, &colors_pull]
        () mutable
        { return block{ colors_pull[color_distr(rng)], std::nullopt }; });
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
            gfx.fill_rectangle(GameEngine::Geometry::Rectangle2D<int>{ LEFT_TOP_POS + BLOCK_SIZE * GameEngine::Geometry::Vector2D<int>{ i % N_BLOCKS_IN_ROW, i / N_BLOCKS_IN_ROW }, BLOCK_SIZE, BLOCK_SIZE }, 
            grid[static_cast<std::size_t>(i)].second.value_or([](GameEngine::Colour c){ return c; })(grid[static_cast<std::size_t>(i)].first));
        }
    }

    constexpr iterator get_block(GameEngine::Geometry::Vector2D<int> coordinate) noexcept
    {
        coordinate -= LEFT_TOP_POS;
        coordinate /= BLOCK_SIZE;

        return iterator{ grid.data() + coordinate.y * N_BLOCKS_IN_ROW + coordinate.x };
    }

    constexpr iterator begin() noexcept
    {
        return iterator{ grid.data() };
    }
    constexpr iterator end()   noexcept
    {
        return iterator{ grid.data() + grid.size() };
    }

    auto get_adject_blocks(iterator block)
    {
        constexpr std::size_t MAX_N_NEIGHBOURS{ 4U };
        static std::array<iterator, MAX_N_NEIGHBOURS> adject_blocks{ };
        std::size_t cur_n_neighbours{ 0U };
        auto const  index{ std::distance(begin(), block) };
        auto const row{ index / N_BLOCKS_IN_ROW };
        auto const col{ index % N_BLOCKS_IN_ROW };

        if (row != 0)                   adject_blocks[cur_n_neighbours++] = iterator{ grid.data() + (row - 1) * N_BLOCKS_IN_ROW + col };
        if (row != N_BLOCKS_IN_ROW)     adject_blocks[cur_n_neighbours++] = iterator{ grid.data() + (row + 1) * N_BLOCKS_IN_ROW + col };
        if (col != 0)                   adject_blocks[cur_n_neighbours++] = iterator{ grid.data() + row * N_BLOCKS_IN_ROW + col - 1 };
        if (col != N_BLOCKS_IN_ROW - 1) adject_blocks[cur_n_neighbours++] = iterator{ grid.data() + row * N_BLOCKS_IN_ROW + col + 1 };

        return std::views::take(adject_blocks, cur_n_neighbours);
    }

private:

    std::array<block, N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW> grid;
};