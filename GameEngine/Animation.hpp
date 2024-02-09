#pragma once

#include "Surface.hpp"
#include "IGraphics2D.hpp"


namespace GameEngine
{
    class Animation
    {
    public:

        static constexpr size_t UNKNOWN_N{ 0U };
        static constexpr Geometry::Vector2D<int> LEFT_TOP{ 0, 0 };

    public:

        enum class FramesAlignment
        {
            HORIZONTAL, VERTICAL
        };

        Animation(std::filesystem::path const& sprites_sheet_src, size_t frame_width, size_t frame_height, float frame_duration, size_t n = UNKNOWN_N, FramesAlignment direction = FramesAlignment::HORIZONTAL, Geometry::Vector2D<int> start_point = LEFT_TOP);

        void update(float dt);
        void draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma) const;

        ~Animation() = default;

    private:

        __forceinline void toggle_frame() noexcept;

    private:

        float const frame_duration;
        float elapsed_duration{ 0.f };

        std::vector<Surface> frames;    
        size_t cur_frame_index;
    };
}