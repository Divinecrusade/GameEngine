#include "KeyColor.hpp"
#include <algorithm>


namespace GameEngine
{
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