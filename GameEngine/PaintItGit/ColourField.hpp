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
public:

    static constexpr int BLOCK_SIZE{ SIZE / N_BLOCKS_IN_ROW };
    static constexpr std::size_t MAX_N_ADJECT_BLOCKS{ 4U };

    using effect = std::optional<std::function<GameEngine::Colour(GameEngine::Colour)>>;
    using block = std::pair<GameEngine::Colour, effect>;
    using iterator = GameEngine::Auxiliary::ContiguousIterator<block, ColourField>;

public:

    ColourField() = default;
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

    std::size_t get_adject_blocks_with_not_equal_color(iterator cur_block, GameEngine::Colour c, std::array<iterator, MAX_N_ADJECT_BLOCKS>& adject_blocks)
    {
        std::size_t cur_n_neighbours{ 0U };
        auto const index{ std::distance(begin(), cur_block) };
        auto const row{ index / N_BLOCKS_IN_ROW };
        auto const col{ index % N_BLOCKS_IN_ROW };

        if (iterator const it{ grid.data() + (row - 1) * N_BLOCKS_IN_ROW + col }; row != 0 && it->first != c)                   adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + (row + 1) * N_BLOCKS_IN_ROW + col }; row != N_BLOCKS_IN_ROW + 1 && it->first != c) adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + row * N_BLOCKS_IN_ROW + col - 1 };   col != 0 && it->first != c)                   adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + row * N_BLOCKS_IN_ROW + col + 1 };   col != N_BLOCKS_IN_ROW - 1 && it->first != c) adject_blocks[cur_n_neighbours++] = it;

        return cur_n_neighbours;
    }

private:

    std::array<block, N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW> grid;
};