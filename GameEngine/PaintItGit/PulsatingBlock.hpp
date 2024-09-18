#pragma once

#include "DrawableBlock.hpp"
#include "PulsationEffect.hpp"

// CUBE
// Выставлять CLI
// Выставлять SEI

template<int SIZE>
requires (SIZE > 0)
class PulsatingBlock : public DrawableBlock<SIZE>
{
public:

    constexpr PulsatingBlock(PulsationEffect const* pulsator = nullptr, bool to_start_pulsation = false, GameEngine::Geometry::Vector2D<int> const& left_top_pos = DrawableBlock<SIZE>::DEFAULT_LEFT_TOP_POS, GameEngine::Colour c = ColourBlock::DEFAULT_COLOUR) noexcept
    :
    DrawableBlock<SIZE>{ left_top_pos, c },
    pulsator{ pulsator },
    pulsation_in_process{ to_start_pulsation }
    { }
    constexpr PulsatingBlock(PulsatingBlock const&) noexcept = default;
    constexpr PulsatingBlock(PulsatingBlock&&)      noexcept = default;

    constexpr PulsatingBlock& operator=(PulsatingBlock const&) noexcept = default;
    constexpr PulsatingBlock& operator=(PulsatingBlock&&)      noexcept = default;

    constexpr ~PulsatingBlock() noexcept = default;


    constexpr void pulsation_toggle() noexcept
    {
        assert(pulsator != nullptr);
        pulsation_in_process = !pulsation_in_process;
    }
    
    constexpr void pulsation_on() noexcept
    {
        assert(pulsator != nullptr);
        pulsation_in_process = true;
    }

    constexpr void pulsation_off() noexcept
    {
        assert(pulsator != nullptr);
        pulsation_in_process = false;
    }

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        assert(pulsator != nullptr);
        if (pulsation_in_process) gfx.fill_rectangle(this->get_collision_box(), (*pulsator)(this->get_colour()));
        else                      DrawableBlock<SIZE>::draw(gfx);
    }

private:

    PulsationEffect const* pulsator;
    bool pulsation_in_process;
};

