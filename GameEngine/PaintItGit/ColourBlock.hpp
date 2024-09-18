#pragma once

#include <KeyColor.hpp>


class ColourBlock
{
public:

    static constexpr GameEngine::Colour DEFAULT_COLOUR{ GameEngine::Colours::BLACK };
    
    constexpr ColourBlock(GameEngine::Colour c = DEFAULT_COLOUR) noexcept
    :
    c{ c }
    { }
    constexpr ColourBlock(ColourBlock const&) noexcept = default;
    constexpr ColourBlock(ColourBlock&&)      noexcept = default;

    constexpr ColourBlock& operator=(ColourBlock const&) noexcept = default;
    constexpr ColourBlock& operator=(ColourBlock&&)      noexcept = default;

    virtual constexpr ~ColourBlock() noexcept = default;


    constexpr GameEngine::Colour get_colour() const noexcept
    {
        return c;
    }

    void set_colour(GameEngine::Colour new_c) noexcept
    {
        c = new_c;
    }

private:

    GameEngine::Colour c;
};