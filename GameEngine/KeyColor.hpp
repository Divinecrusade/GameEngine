#pragma once

#include <cstdint>
#include <algorithm>


namespace GameEngine
{
    union KeyColor final
    {
    public:

        [[maybe_unused]] static constexpr uint8_t MIN_COLOUR_DEPTH{ 0U };
        [[maybe_unused]] static constexpr uint8_t MAX_COLOUR_DEPTH{ 255U };

        enum class ComponentIndex : std::size_t
        {
            R = 0U,
            G = 1U,
            B = 2U,
            A = 3U
        };

        [[maybe_unused]] static constexpr uint32_t encode(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
        {
            uint32_t rgba{ 0U };
            uint32_t r_{ static_cast<uint32_t>(r) };
            uint32_t g_{ static_cast<uint32_t>(g) << 8U };
            uint32_t b_{ static_cast<uint32_t>(b) << 16U };
            uint32_t a_{ static_cast<uint32_t>(a) << 24U };

            return rgba | r_ | g_ | b_ | a_;
        }

        [[maybe_unused]] static constexpr bool is_equal_except_one_component(KeyColor const& lhs, KeyColor const& rhs, ComponentIndex ignored_component = ComponentIndex::A)
        {
            KeyColor const xored_colour{ lhs.rgba ^ rhs.rgba };
            KeyColor mask{ };
            mask[ignored_component] = xored_colour[ignored_component];

            return xored_colour.rgba == mask.rgba;
        }

        [[maybe_unused]] constexpr KeyColor() noexcept = default;
        [[maybe_unused]] constexpr KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_COLOUR_DEPTH) noexcept
        :
        rgba{ encode(r, g, b, a) }
        { }
        [[maybe_unused]] constexpr KeyColor(uint32_t encoded) noexcept
        :
        rgba{ encoded }
        { }
        [[maybe_unused]] constexpr KeyColor(KeyColor const& c) noexcept
        :
        rgba{ c.rgba }
        { }
        [[maybe_unused]] KeyColor(KeyColor&& c) noexcept
        {
            std::swap(rgba, c.rgba);
        }
        [[maybe_unused]] constexpr KeyColor& operator=(KeyColor const& c) noexcept
        {
            rgba = c.rgba;

            return *this;
        }
        [[maybe_unused]] KeyColor& operator=(KeyColor&& c) noexcept
        {
            std::swap(rgba, c.rgba);

            return *this;
        }
        [[maybe_unused]] ~KeyColor() noexcept = default;

        [[maybe_unused]] constexpr uint32_t get_encoded() const noexcept
        {
            return rgba;
        }
        
        [[maybe_unused]] constexpr uint8_t& operator[](ComponentIndex index) noexcept
        {
            return colors[static_cast<std::size_t>(index)];
        }
        [[maybe_unused]] constexpr uint8_t const& operator[](ComponentIndex index) const noexcept
        {
            return colors[static_cast<std::size_t>(index)];
        }

        [[maybe_unused]] static constexpr KeyColor blend(KeyColor const& beg, KeyColor const& end, uint8_t a) noexcept
        {
            return KeyColor
            { 
                static_cast<uint8_t>((end[ComponentIndex::R] - beg[ComponentIndex::R]) * a / 100 + beg[ComponentIndex::R]),
                static_cast<uint8_t>((end[ComponentIndex::G] - beg[ComponentIndex::G]) * a / 100 + beg[ComponentIndex::G]),
                static_cast<uint8_t>((end[ComponentIndex::B] - beg[ComponentIndex::B]) * a / 100 + beg[ComponentIndex::B]),
            };
        }

        uint8_t colors[4U];
        uint32_t rgba{ 0U };
    };

    [[maybe_unused]] constexpr bool operator==(KeyColor const& lhs, KeyColor const& rhs) noexcept
    {
        return lhs.get_encoded() == rhs.get_encoded();
    }
    [[maybe_unused]] constexpr bool operator!=(KeyColor const& lhs, KeyColor const& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    using Colour = KeyColor;

    struct Colours final
    {
        [[maybe_unused]] static constexpr Colour RED     { Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour GREEN   { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour BLUE    { Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour BLACK   { Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour WHITE   { Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour CYAN    { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour MAGENTA { Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour YELLOW  { Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        [[maybe_unused]] static constexpr Colour AQUA    { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };

        [[maybe_unused]] static constexpr Colour ALICE_BLUE{ 240U, 248U, 255U };
        [[maybe_unused]] static constexpr Colour ANTIQUE_WHITE{ 250U, 235U, 215U };
        [[maybe_unused]] static constexpr Colour AQUAMARINE{ 127U, 255U, 212U };
        [[maybe_unused]] static constexpr Colour AZURE{ 240U, 255U, 255U };
        [[maybe_unused]] static constexpr Colour BEIGE{ 245U, 245U, 220U };
        [[maybe_unused]] static constexpr Colour BISQUE{ 255U, 228U, 196U };
        [[maybe_unused]] static constexpr Colour BLANCHED_ALMOND{ 255U, 235U, 205U };
        [[maybe_unused]] static constexpr Colour BLUE_VIOLET{ 138U, 43U, 226U };
        [[maybe_unused]] static constexpr Colour BROWN{ 165U, 42U, 42U };
        [[maybe_unused]] static constexpr Colour BURLYWOOD{ 222U, 184U, 135U };
        [[maybe_unused]] static constexpr Colour CADET_BLUE{ 95U, 158U, 160U };
        [[maybe_unused]] static constexpr Colour CHARTREUSE{ 127U, 255U, 0U };
        [[maybe_unused]] static constexpr Colour CHOCOLATE{ 210U, 105U, 30U };
        [[maybe_unused]] static constexpr Colour CORAL{ 255U, 127U, 80U };
        [[maybe_unused]] static constexpr Colour CORNFLOWER_BLUE{ 100U, 149U, 237U };
        [[maybe_unused]] static constexpr Colour CORNSILK{ 255U, 248U, 220U };
        [[maybe_unused]] static constexpr Colour CRIMSON{ 220U, 20U, 60U };
        [[maybe_unused]] static constexpr Colour DARK_BLUE{ 0U, 0U, 139U };
        [[maybe_unused]] static constexpr Colour DARK_CYAN{ 0U, 139U, 139U };
        [[maybe_unused]] static constexpr Colour DARK_GOLDEN_ROD{ 184U, 134U, 11U };
        [[maybe_unused]] static constexpr Colour DARK_GREY{ 169U, 169U, 169U };
        [[maybe_unused]] static constexpr Colour DARK_GREEN{ 0U, 100U, 0U };
        [[maybe_unused]] static constexpr Colour DARK_KHAKI{ 189U, 183U, 107U };
        [[maybe_unused]] static constexpr Colour DARK_MAGENTA{ 139U, 0U, 139U };
        [[maybe_unused]] static constexpr Colour DARK_OLIVE_GREEN{ 85U, 107U, 47U };
        [[maybe_unused]] static constexpr Colour DARK_ORANGE{ 255U, 140U, 0U };
        [[maybe_unused]] static constexpr Colour DARK_ORCHID{ 153U, 50U, 204U };
        [[maybe_unused]] static constexpr Colour DARK_RED{ 139U, 0U, 0U };
        [[maybe_unused]] static constexpr Colour DARK_SALMON{ 233U, 150U, 122U };
        [[maybe_unused]] static constexpr Colour DARK_SEA_GREEN{ 143U, 188U, 143U };
        [[maybe_unused]] static constexpr Colour DARK_SLATE_BLUE{ 72U, 61U, 139U };
        [[maybe_unused]] static constexpr Colour DARK_SLATE_GREY{ 47U, 79U, 79U };
        [[maybe_unused]] static constexpr Colour DARK_TURQUOISE{ 0U, 206U, 209U };
        [[maybe_unused]] static constexpr Colour DARK_VIOLET{ 148U, 0U, 211U };
        [[maybe_unused]] static constexpr Colour DEEP_PINK{ 255U, 20U, 147U };
        [[maybe_unused]] static constexpr Colour DEEP_SKY_BLUE{ 0U, 191U, 255U };
        [[maybe_unused]] static constexpr Colour DIM_GREY{ 105U, 105U, 105U };
        [[maybe_unused]] static constexpr Colour DODGER_BLUE{ 30U, 144U, 255U };
        [[maybe_unused]] static constexpr Colour FIRE_BRICK{ 178U, 34U, 34U };
        [[maybe_unused]] static constexpr Colour FLORAL_WHITE{ 255U, 250U, 240U };
        [[maybe_unused]] static constexpr Colour FOREST_GREEN{ 34U, 139U, 34U };
        [[maybe_unused]] static constexpr Colour GAINSBORO{ 220U, 220U, 220U };
        [[maybe_unused]] static constexpr Colour GHOST_WHITE{ 248U, 248U, 255U };
        [[maybe_unused]] static constexpr Colour GOLD{ 255U, 215U, 0U };
        [[maybe_unused]] static constexpr Colour GOLDEN_ROD{ 218U, 165U, 32U };
        [[maybe_unused]] static constexpr Colour GREEN_YELLOW{ 173U, 255U, 47U };
        [[maybe_unused]] static constexpr Colour GREY{ 128U, 128U, 128U };
        [[maybe_unused]] static constexpr Colour HONEYDEW{ 240U, 255U, 240U };
        [[maybe_unused]] static constexpr Colour HOT_PINK{ 255U, 105U, 180U };
        [[maybe_unused]] static constexpr Colour INDIAN_RED{ 205U, 92U, 92U };
        [[maybe_unused]] static constexpr Colour INDIGO{ 75U, 0U, 130U };
        [[maybe_unused]] static constexpr Colour IVORY{ 255U, 255U, 240U };
        [[maybe_unused]] static constexpr Colour KHAKI{ 240U, 230U, 140U };
        [[maybe_unused]] static constexpr Colour LAVENDER{ 230U, 230U, 250U };
        [[maybe_unused]] static constexpr Colour LAVENDER_BLUSH{ 255U, 240U, 245U };
        [[maybe_unused]] static constexpr Colour LAWN_GREEN{ 124U, 252U, 0U };
        [[maybe_unused]] static constexpr Colour LEMON_CHIFFON{ 255U, 250U, 205U };
        [[maybe_unused]] static constexpr Colour LIGHT_BLUE{ 173U, 216U, 230U };
        [[maybe_unused]] static constexpr Colour LIGHT_CORAL{ 240U, 128U, 128U };
        [[maybe_unused]] static constexpr Colour LIGHT_CYAN{ 224U, 255U, 255U };
        [[maybe_unused]] static constexpr Colour LIGHT_GOLDEN_ROD_YELLOW{ 250U, 250U, 210U };
        [[maybe_unused]] static constexpr Colour LIGHT_GREY{ 211U, 211U, 211U };
        [[maybe_unused]] static constexpr Colour LIGHT_GREEN{ 144U, 238U, 144U };
        [[maybe_unused]] static constexpr Colour LIGHT_PINK{ 255U, 182U, 193U };
        [[maybe_unused]] static constexpr Colour LIGHT_SALMON{ 255U, 160U, 122U };
        [[maybe_unused]] static constexpr Colour LIGHT_SEA_GREEN{ 32U, 178U, 170U };
        [[maybe_unused]] static constexpr Colour LIGHT_SKY_BLUE{ 135U, 206U, 250U };
        [[maybe_unused]] static constexpr Colour LIGHT_SLATE_GREY{ 119U, 136U, 153U };
        [[maybe_unused]] static constexpr Colour LIGHT_STEEL_BLUE{ 176U, 196U, 222U };
        [[maybe_unused]] static constexpr Colour LIGHT_YELLOW{ 255U, 255U, 224U };
        [[maybe_unused]] static constexpr Colour LIME{ 0U, 255U, 0U };
        [[maybe_unused]] static constexpr Colour LIME_GREEN{ 50U, 205U, 50U };
        [[maybe_unused]] static constexpr Colour LINEN{ 250U, 240U, 230U };
        [[maybe_unused]] static constexpr Colour MAROON{ 128U, 0U, 0U };
        [[maybe_unused]] static constexpr Colour MEDIUM_AQUAMARINE{ 102U, 205U, 170U };
        [[maybe_unused]] static constexpr Colour MEDIUM_BLUE{ 0U, 0U, 205U };
        [[maybe_unused]] static constexpr Colour MEDIUM_ORCHID{ 186U, 85U, 211U };
        [[maybe_unused]] static constexpr Colour MEDIUM_PURPLE{ 147U, 112U, 219U };
        [[maybe_unused]] static constexpr Colour MEDIUM_SEA_GREEN{ 60U, 179U, 113U };
        [[maybe_unused]] static constexpr Colour MEDIUM_SLATE_BLUE{ 123U, 104U, 238U };
        [[maybe_unused]] static constexpr Colour MEDIUM_SPRING_GREEN{ 0U, 250U, 154U };
        [[maybe_unused]] static constexpr Colour MEDIUM_TURQUOISE{ 72U, 209U, 204U };
        [[maybe_unused]] static constexpr Colour MEDIUM_VIOLET_RED{ 199U, 21U, 133U };
        [[maybe_unused]] static constexpr Colour MIDNIGHT_BLUE{ 25U, 25U, 112U };
        [[maybe_unused]] static constexpr Colour MINT_CREAM{ 245U, 255U, 250U };
        [[maybe_unused]] static constexpr Colour MISTY_ROSE{ 255U, 228U, 225U };
        [[maybe_unused]] static constexpr Colour MOCCASIN{ 255U, 228U, 181U };
        [[maybe_unused]] static constexpr Colour NAVAJO_WHITE{ 255U, 222U, 173U };
        [[maybe_unused]] static constexpr Colour NAVY{ 0U, 0U, 128U };
        [[maybe_unused]] static constexpr Colour OLD_LACE{ 253U, 245U, 230U };
        [[maybe_unused]] static constexpr Colour OLIVE{ 128U, 128U, 0U };
        [[maybe_unused]] static constexpr Colour OLIVE_DRAB{ 107U, 142U, 35U };
        [[maybe_unused]] static constexpr Colour ORANGE{ 255U, 165U, 0U };
        [[maybe_unused]] static constexpr Colour ORANGE_RED{ 255U, 69U, 0U };
        [[maybe_unused]] static constexpr Colour ORCHID{ 218U, 112U, 214U };
        [[maybe_unused]] static constexpr Colour PALE_GOLDEN_ROD{ 238U, 232U, 170U };
        [[maybe_unused]] static constexpr Colour PALE_GREEN{ 152U, 251U, 152U };
        [[maybe_unused]] static constexpr Colour PALE_TURQUOISE{ 175U, 238U, 238U };
        [[maybe_unused]] static constexpr Colour PALE_VIOLET_RED{ 219U, 112U, 147U };
        [[maybe_unused]] static constexpr Colour PAPAYA_WHIP{ 255U, 239U, 213U };
        [[maybe_unused]] static constexpr Colour PEACH_PUFF{ 255U, 218U, 185U };
        [[maybe_unused]] static constexpr Colour PERU{ 205U, 133U, 63U };
        [[maybe_unused]] static constexpr Colour PINK{ 255U, 192U, 203U };
        [[maybe_unused]] static constexpr Colour PLUM{ 221U, 160U, 221U };
        [[maybe_unused]] static constexpr Colour POWDER_BLUE{ 176U, 224U, 230U };
        [[maybe_unused]] static constexpr Colour PURPLE{ 128U, 0U, 128U };
        [[maybe_unused]] static constexpr Colour ROSY_BROWN{ 188U, 143U, 143U };
        [[maybe_unused]] static constexpr Colour ROYAL_BLUE{ 65U, 105U, 225U };
        [[maybe_unused]] static constexpr Colour SADDLE_BROWN{ 139U, 69U, 19U };
        [[maybe_unused]] static constexpr Colour SALMON{ 250U, 128U, 114U };
        [[maybe_unused]] static constexpr Colour SANDY_BROWN{ 244U, 164U, 96U };
        [[maybe_unused]] static constexpr Colour SEA_GREEN{ 46U, 139U, 87U };
        [[maybe_unused]] static constexpr Colour SEA_SHELL{ 255U, 245U, 238U };
        [[maybe_unused]] static constexpr Colour SIENNA{ 160U, 82U, 45U };
        [[maybe_unused]] static constexpr Colour SILVER{ 192U, 192U, 192U };
        [[maybe_unused]] static constexpr Colour SKY_BLUE{ 135U, 206U, 235U };
        [[maybe_unused]] static constexpr Colour SLATE_BLUE{ 106U, 90U, 205U };
        [[maybe_unused]] static constexpr Colour SLATE_GREY{ 112U, 128U, 144U };
        [[maybe_unused]] static constexpr Colour SNOW{ 255U, 255U, 250U };
        [[maybe_unused]] static constexpr Colour SPRING_GREEN{ 0U, 255U, 127U };
        [[maybe_unused]] static constexpr Colour STEEL_BLUE{ 70U, 130U, 180U };
        [[maybe_unused]] static constexpr Colour TAN{ 210U, 180U, 140U };
        [[maybe_unused]] static constexpr Colour TEAL{ 0U, 128U, 128U };
        [[maybe_unused]] static constexpr Colour THISTLE{ 216U, 191U, 216U };
        [[maybe_unused]] static constexpr Colour TOMATO{ 255U, 99U, 71U };
        [[maybe_unused]] static constexpr Colour TURQUOISE{ 64U, 224U, 208U };
        [[maybe_unused]] static constexpr Colour VIOLET{ 238U, 130U, 238U };
        [[maybe_unused]] static constexpr Colour WHEAT{ 245U, 222U, 179U };
        [[maybe_unused]] static constexpr Colour WHITE_SMOKE{ 245U, 245U, 245U };
        [[maybe_unused]] static constexpr Colour YELLOW_GREEN{ 154U, 205U, 50U };
    };
}