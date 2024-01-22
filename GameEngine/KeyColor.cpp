#include "KeyColor.hpp"
#include <algorithm>


namespace GameEngine2D
{
    KeyColor::KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
    {
        colors[R_INDEX] = r;
        colors[G_INDEX] = g;
        colors[B_INDEX] = b;
        colors[A_INDEX] = a;
    }

    KeyColor::KeyColor(KeyColor const& c) noexcept
    {
        rgba = c.rgba;
    }

    KeyColor::KeyColor(KeyColor&& c) noexcept
    {
        std::swap(rgba, c.rgba);
    }

    KeyColor& KeyColor::operator=(KeyColor const& c) noexcept
    {
        rgba = c.rgba;

        return *this;
    }

    KeyColor& KeyColor::operator=(KeyColor&& c) noexcept
    {
        std::swap(rgba, c.rgba);

        return *this;
    }

    uint32_t KeyColor::get_encoded() const noexcept
    {
        return rgba;
    }

    bool operator==(KeyColor const& lhs, KeyColor const& rhs)
    {
        return lhs.get_encoded() == rhs.get_encoded();
    }
}