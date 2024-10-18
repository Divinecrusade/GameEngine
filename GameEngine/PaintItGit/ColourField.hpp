#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <IDrawable.hpp>
#include <ContiguousIterator.hpp>

#include <array>
#include <algorithm>
#include <ranges>
#include <random>

#include "PulsatingBlock.hpp"


template<int SIZE, int N_BLOCKS_IN_ROW, GameEngine::Geometry::Vector2D<int> LEFT_TOP_POS>
requires (SIZE > 0 && N_BLOCKS_IN_ROW > 0 && SIZE % N_BLOCKS_IN_ROW == 0)
class ColourField final : public GameEngine::Interfaces::IDrawable
{
public:

    static constexpr int BLOCK_SIZE{ SIZE / N_BLOCKS_IN_ROW };
    static constexpr std::size_t MAX_N_ADJECT_BLOCKS{ 4U };

    using iterator = GameEngine::Auxiliary::ContiguousIterator<PulsatingBlock<BLOCK_SIZE>, ColourField>;

public:

    ColourField() = delete;
    template<std::size_t N>
    requires (N > 0U)
    constexpr ColourField(PulsationEffect const* pulsator, std::array<GameEngine::Colour, N> const& colours_pull, std::size_t cur_colour_index) noexcept
    {
        assert(pulsator != nullptr);
        assert(cur_colour_index < N);

        std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<std::size_t> colour_indecies_distr{ 0U, N - 1U };

        for (int i{ 0 }; i != N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW; ++i)
        {
            GameEngine::Colour const c{ colours_pull[colour_indecies_distr(rng)] };

            grid[i] = PulsatingBlock<BLOCK_SIZE>{ pulsator, c != colours_pull[cur_colour_index], LEFT_TOP_POS + BLOCK_SIZE * GameEngine::Geometry::Vector2D<int>{ i% N_BLOCKS_IN_ROW, i / N_BLOCKS_IN_ROW }, c };
        }
    }
    ColourField(ColourField const&) = delete;
    ColourField(ColourField&&)      = delete;

    ColourField& operator=(ColourField const&) = delete;
    ColourField& operator=(ColourField&&)      = delete;

    constexpr virtual ~ColourField() = default;


    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        for (auto const& block : grid)
        {
            block.draw(gfx);
        }
    }

    constexpr iterator get_block(GameEngine::Geometry::Vector2D<int> coordinate) noexcept
    {
        coordinate -= LEFT_TOP_POS;
        coordinate /= BLOCK_SIZE;

        assert(coordinate.x >= 0 && coordinate.x < N_BLOCKS_IN_ROW);
        assert(coordinate.y >= 0 && coordinate.y < N_BLOCKS_IN_ROW);

        return std::to_address(grid.begin() + coordinate.y * N_BLOCKS_IN_ROW + coordinate.x);
    }
    
    constexpr iterator get_iterator(PulsatingBlock<BLOCK_SIZE>* ptr) noexcept
    {
        assert(ptr >= std::to_address(begin()));
        assert(ptr <= std::to_address(end()));
        
        return iterator{ ptr };
    }

    constexpr iterator begin() noexcept
    {
        return std::to_address(grid.begin());
    }
    constexpr iterator end()   noexcept
    {
        return std::to_address(grid.end());
    }

    constexpr std::size_t get_adject_blocks(iterator cur_block, std::array<iterator, MAX_N_ADJECT_BLOCKS>& adject_blocks, std::function<bool(GameEngine::Colour)> const& filter)
    {
        assert(cur_block >= this->begin());
        assert(cur_block < this->end());

        std::size_t cur_n_neighbours{ 0U };
        auto const index{ std::distance(this->begin(), cur_block) };
        auto const row{ index / N_BLOCKS_IN_ROW };
        auto const col{ index % N_BLOCKS_IN_ROW };

        if (iterator const it{ grid.data() + (row - 1) * N_BLOCKS_IN_ROW + col }; row != 0                   && filter(it->get_colour())) adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + (row + 1) * N_BLOCKS_IN_ROW + col }; row != N_BLOCKS_IN_ROW - 1 && filter(it->get_colour())) adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + row * N_BLOCKS_IN_ROW + col - 1 };   col != 0                   && filter(it->get_colour())) adject_blocks[cur_n_neighbours++] = it;
        if (iterator const it{ grid.data() + row * N_BLOCKS_IN_ROW + col + 1 };   col != N_BLOCKS_IN_ROW - 1 && filter(it->get_colour())) adject_blocks[cur_n_neighbours++] = it;

        return cur_n_neighbours;
    }

    std::function<std::size_t(ColourBlock&)> get_serializer() const noexcept
    {
        return [this](ColourBlock& block) -> std::size_t 
        { 
            return std::distance(grid.begin(), std::ranges::find_if(grid, [p = &block](auto const& val){ return p == &val; }));
        };
    }

    std::function<ColourBlock& (std::size_t)> get_deserializer() noexcept
    {
        return [this](std::size_t i) -> ColourBlock& { return *reinterpret_cast<ColourBlock*>(&(grid[i])); };
    }


private:

    std::array<PulsatingBlock<BLOCK_SIZE>, N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW> grid;
};