#pragma once

#include <cstdint>

namespace GameEngine
{
    union KeyColor
    {
    private:

        static constexpr uint32_t encode(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
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

        constexpr KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_COLOUR_DEPTH) noexcept
        :
        rgba{ encode(r, g, b, a) }
        { }
        constexpr KeyColor(uint32_t encoded)
        :
        rgba{ encoded }
        { }
        constexpr KeyColor(KeyColor const& c) noexcept
        :
        rgba{ c.rgba }
        { }
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
        static constexpr Colour RED     { Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour GREEN   { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour BLUE    { Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour BLACK   { Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour WHITE   { Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour CYAN    { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour MAGENTA { Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };
        static constexpr Colour YELLOW  { Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MIN_COLOUR_DEPTH };
        static constexpr Colour AQUA    { Colour::MIN_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH, Colour::MAX_COLOUR_DEPTH };

        static constexpr Colour ALICE_BLUE{ 240U, 248U, 255U };
        static constexpr Colour ANTIQUE_WHITE{ 250U, 235U, 215U };
        static constexpr Colour AQUAMARINE{ 127U, 255U, 212U };
        static constexpr Colour AZURE{ 240U, 255U, 255U };
        static constexpr Colour BEIGE{ 245U, 245U, 220U };
        static constexpr Colour BISQUE{ 255U, 228U, 196U };
        static constexpr Colour BLANCHED_ALMOND{ 255U, 235U, 205U };
        static constexpr Colour BLUE_VIOLET{ 138U, 43U, 226U };
        static constexpr Colour BROWN{ 165U, 42U, 42U };
        static constexpr Colour BURLYWOOD{ 222U, 184U, 135U };
        static constexpr Colour CADET_BLUE{ 95U, 158U, 160U };
        static constexpr Colour CHARTREUSE{ 127U, 255U, 0U };
        static constexpr Colour CHOCOLATE{ 210U, 105U, 30U };
        static constexpr Colour CORAL{ 255U, 127U, 80U };
        static constexpr Colour CORNFLOWER_BLUE{ 100U, 149U, 237U };
        static constexpr Colour CORNSILK{ 255U, 248U, 220U };
        static constexpr Colour CRIMSON{ 220U, 20U, 60U };
        static constexpr Colour DARK_BLUE{ 0U, 0U, 139U };
        static constexpr Colour DARK_CYAN{ 0U, 139U, 139U };
        static constexpr Colour DARK_GOLDEN_ROD{ 184U, 134U, 11U };
        static constexpr Colour DARK_GREY{ 169U, 169U, 169U };
        static constexpr Colour DARK_GREEN{ 0U, 100U, 0U };
        static constexpr Colour DARK_KHAKI{ 189U, 183U, 107U };
        static constexpr Colour DARK_MAGENTA{ 139U, 0U, 139U };
        static constexpr Colour DARK_OLIVE_GREEN{ 85U, 107U, 47U };
        static constexpr Colour DARK_ORANGE{ 255U, 140U, 0U };
        static constexpr Colour DARK_ORCHID{ 153U, 50U, 204U };
        static constexpr Colour DARK_RED{ 139U, 0U, 0U };
        static constexpr Colour DARK_SALMON{ 233U, 150U, 122U };
        static constexpr Colour DARK_SEA_GREEN{ 143U, 188U, 143U };
        static constexpr Colour DARK_SLATE_BLUE{ 72U, 61U, 139U };
        static constexpr Colour DARK_SLATE_GREY{ 47U, 79U, 79U };
        static constexpr Colour DARK_TURQUOISE{ 0U, 206U, 209U };
        static constexpr Colour DARK_VIOLET{ 148U, 0U, 211U };
        static constexpr Colour DEEP_PINK{ 255U, 20U, 147U };
        static constexpr Colour DEEP_SKY_BLUE{ 0U, 191U, 255U };
        static constexpr Colour DIM_GREY{ 105U, 105U, 105U };
        static constexpr Colour DODGER_BLUE{ 30U, 144U, 255U };
        static constexpr Colour FIRE_BRICK{ 178U, 34U, 34U };
        static constexpr Colour FLORAL_WHITE{ 255U, 250U, 240U };
        static constexpr Colour FOREST_GREEN{ 34U, 139U, 34U };
        static constexpr Colour GAINSBORO{ 220U, 220U, 220U };
        static constexpr Colour GHOST_WHITE{ 248U, 248U, 255U };
        static constexpr Colour GOLD{ 255U, 215U, 0U };
        static constexpr Colour GOLDEN_ROD{ 218U, 165U, 32U };
        static constexpr Colour GREEN_YELLOW{ 173U, 255U, 47U };
        static constexpr Colour GREY{ 128U, 128U, 128U };
        static constexpr Colour HONEYDEW{ 240U, 255U, 240U };
        static constexpr Colour HOT_PINK{ 255U, 105U, 180U };
        static constexpr Colour INDIAN_RED{ 205U, 92U, 92U };
        static constexpr Colour INDIGO{ 75U, 0U, 130U };
        static constexpr Colour IVORY{ 255U, 255U, 240U };
        static constexpr Colour KHAKI{ 240U, 230U, 140U };
        static constexpr Colour LAVENDER{ 230U, 230U, 250U };
        static constexpr Colour LAVENDER_BLUSH{ 255U, 240U, 245U };
        static constexpr Colour LAWN_GREEN{ 124U, 252U, 0U };
        static constexpr Colour LEMON_CHIFFON{ 255U, 250U, 205U };
        static constexpr Colour LIGHT_BLUE{ 173U, 216U, 230U };
        static constexpr Colour LIGHT_CORAL{ 240U, 128U, 128U };
        static constexpr Colour LIGHT_CYAN{ 224U, 255U, 255U };
        static constexpr Colour LIGHT_GOLDEN_ROD_YELLOW{ 250U, 250U, 210U };
        static constexpr Colour LIGHT_GREY{ 211U, 211U, 211U };
        static constexpr Colour LIGHT_GREEN{ 144U, 238U, 144U };
        static constexpr Colour LIGHT_PINK{ 255U, 182U, 193U };
        static constexpr Colour LIGHT_SALMON{ 255U, 160U, 122U };
        static constexpr Colour LIGHT_SEA_GREEN{ 32U, 178U, 170U };
        static constexpr Colour LIGHT_SKY_BLUE{ 135U, 206U, 250U };
        static constexpr Colour LIGHT_SLATE_GREY{ 119U, 136U, 153U };
        static constexpr Colour LIGHT_STEEL_BLUE{ 176U, 196U, 222U };
        static constexpr Colour LIGHT_YELLOW{ 255U, 255U, 224U };
        static constexpr Colour LIME{ 0U, 255U, 0U };
        static constexpr Colour LIME_GREEN{ 50U, 205U, 50U };
        static constexpr Colour LINEN{ 250U, 240U, 230U };
        static constexpr Colour MAROON{ 128U, 0U, 0U };
        static constexpr Colour MEDIUM_AQUAMARINE{ 102U, 205U, 170U };
        static constexpr Colour MEDIUM_BLUE{ 0U, 0U, 205U };
        static constexpr Colour MEDIUM_ORCHID{ 186U, 85U, 211U };
        static constexpr Colour MEDIUM_PURPLE{ 147U, 112U, 219U };
        static constexpr Colour MEDIUM_SEA_GREEN{ 60U, 179U, 113U };
        static constexpr Colour MEDIUM_SLATE_BLUE{ 123U, 104U, 238U };
        static constexpr Colour MEDIUM_SPRING_GREEN{ 0U, 250U, 154U };
        static constexpr Colour MEDIUM_TURQUOISE{ 72U, 209U, 204U };
        static constexpr Colour MEDIUM_VIOLET_RED{ 199U, 21U, 133U };
        static constexpr Colour MIDNIGHT_BLUE{ 25U, 25U, 112U };
        static constexpr Colour MINT_CREAM{ 245U, 255U, 250U };
        static constexpr Colour MISTY_ROSE{ 255U, 228U, 225U };
        static constexpr Colour MOCCASIN{ 255U, 228U, 181U };
        static constexpr Colour NAVAJO_WHITE{ 255U, 222U, 173U };
        static constexpr Colour NAVY{ 0U, 0U, 128U };
        static constexpr Colour OLD_LACE{ 253U, 245U, 230U };
        static constexpr Colour OLIVE{ 128U, 128U, 0U };
        static constexpr Colour OLIVE_DRAB{ 107U, 142U, 35U };
        static constexpr Colour ORANGE{ 255U, 165U, 0U };
        static constexpr Colour ORANGE_RED{ 255U, 69U, 0U };
        static constexpr Colour ORCHID{ 218U, 112U, 214U };
        static constexpr Colour PALE_GOLDEN_ROD{ 238U, 232U, 170U };
        static constexpr Colour PALE_GREEN{ 152U, 251U, 152U };
        static constexpr Colour PALE_TURQUOISE{ 175U, 238U, 238U };
        static constexpr Colour PALE_VIOLET_RED{ 219U, 112U, 147U };
        static constexpr Colour PAPAYA_WHIP{ 255U, 239U, 213U };
        static constexpr Colour PEACH_PUFF{ 255U, 218U, 185U };
        static constexpr Colour PERU{ 205U, 133U, 63U };
        static constexpr Colour PINK{ 255U, 192U, 203U };
        static constexpr Colour PLUM{ 221U, 160U, 221U };
        static constexpr Colour POWDER_BLUE{ 176U, 224U, 230U };
        static constexpr Colour PURPLE{ 128U, 0U, 128U };
        static constexpr Colour ROSY_BROWN{ 188U, 143U, 143U };
        static constexpr Colour ROYAL_BLUE{ 65U, 105U, 225U };
        static constexpr Colour SADDLE_BROWN{ 139U, 69U, 19U };
        static constexpr Colour SALMON{ 250U, 128U, 114U };
        static constexpr Colour SANDY_BROWN{ 244U, 164U, 96U };
        static constexpr Colour SEA_GREEN{ 46U, 139U, 87U };
        static constexpr Colour SEA_SHELL{ 255U, 245U, 238U };
        static constexpr Colour SIENNA{ 160U, 82U, 45U };
        static constexpr Colour SILVER{ 192U, 192U, 192U };
        static constexpr Colour SKY_BLUE{ 135U, 206U, 235U };
        static constexpr Colour SLATE_BLUE{ 106U, 90U, 205U };
        static constexpr Colour SLATE_GREY{ 112U, 128U, 144U };
        static constexpr Colour SNOW{ 255U, 255U, 250U };
        static constexpr Colour SPRING_GREEN{ 0U, 255U, 127U };
        static constexpr Colour STEEL_BLUE{ 70U, 130U, 180U };
        static constexpr Colour TAN{ 210U, 180U, 140U };
        static constexpr Colour TEAL{ 0U, 128U, 128U };
        static constexpr Colour THISTLE{ 216U, 191U, 216U };
        static constexpr Colour TOMATO{ 255U, 99U, 71U };
        static constexpr Colour TURQUOISE{ 64U, 224U, 208U };
        static constexpr Colour VIOLET{ 238U, 130U, 238U };
        static constexpr Colour WHEAT{ 245U, 222U, 179U };
        static constexpr Colour WHITE_SMOKE{ 245U, 245U, 245U };
        static constexpr Colour YELLOW_GREEN{ 154U, 205U, 50U };
    }
}