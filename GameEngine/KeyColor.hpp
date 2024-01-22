#pragma once

#include <cstdint>

namespace GameEngine2D
{
    enum class ConstEval
    {
        COMPILE_TIME
    };

    union KeyColor
    {
    private:

        static consteval uint32_t encode(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
        {
            uint32_t rgba{ 0U };
            uint32_t r_  { static_cast<uint32_t>(r) };
            uint32_t g_  { static_cast<uint32_t>(g) << 8U  };
            uint32_t b_  { static_cast<uint32_t>(b) << 16U };
            uint32_t a_  { static_cast<uint32_t>(a) << 24U };

            return rgba | r_ | g_ | b_ | a_;
        }

    public:

        static constexpr uint8_t MIN_COLOUR_DEPTH{ 0U };
        static constexpr uint8_t MAX_COLOUR_DEPTH{ 255U };

        static constexpr size_t  R_INDEX{ 0U };
        static constexpr size_t  G_INDEX{ 1U };
        static constexpr size_t  B_INDEX{ 2U };
        static constexpr size_t  A_INDEX{ 3U };

        consteval KeyColor(ConstEval, uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_COLOUR_DEPTH) noexcept
        :
        rgba{ encode(r, g, b, a) }
        { }
        KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_COLOUR_DEPTH) noexcept;
        KeyColor(uint32_t encoded)
        :
        rgba{ encoded }
        { }
        KeyColor(KeyColor const& c) noexcept;
        KeyColor(KeyColor&& c) noexcept;
        KeyColor& operator=(KeyColor const& c) noexcept;
        KeyColor& operator=(KeyColor&& c) noexcept;
        ~KeyColor() = default;

        uint32_t get_encoded() const noexcept;
        
    public:

        uint8_t colors[4U];

    private:

        uint32_t rgba{ 0U };
    };

    bool operator==(KeyColor const& lhs, KeyColor const& rhs);

    using Colour = KeyColor;

    namespace Colours
    {
        static constexpr Colour RED     { ConstEval::COMPILE_TIME, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour GREEN   { ConstEval::COMPILE_TIME, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour BLUE    { ConstEval::COMPILE_TIME, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour BLACK   { ConstEval::COMPILE_TIME, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour WHITE   { ConstEval::COMPILE_TIME, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour CYAN    { ConstEval::COMPILE_TIME, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour MAGENTA { ConstEval::COMPILE_TIME, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour YELLOW  { ConstEval::COMPILE_TIME, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
    }
}