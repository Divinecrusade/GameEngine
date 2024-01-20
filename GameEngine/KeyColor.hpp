#pragma once

#include <cstdint>

namespace GameEngine2D
{
    union KeyColor
    {
    public:

        KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
        KeyColor(KeyColor const& c) noexcept;
        KeyColor(KeyColor&& c) noexcept;
        KeyColor& operator=(KeyColor const& c) noexcept;
        KeyColor& operator=(KeyColor&& c) noexcept;
        ~KeyColor() = default;

        uint32_t get_encoded() const noexcept;

    private:

        uint8_t colors[4U];
        uint32_t rgba{ 0U };
    };

    bool operator==(KeyColor const& lhs, KeyColor const& rhs);
}