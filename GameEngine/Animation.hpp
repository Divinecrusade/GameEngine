#pragma once

#include "Surface.hpp"
#include "SurfaceView.hpp"
#include "IGraphics2D.hpp"


namespace GameEngine
{
    class Animation
    {
    public:

        enum class FramesAlignment
        {
            HORIZONTAL, VERTICAL
        };

        static constexpr Geometry::Vector2D<int> DEFAULT_START_POINT{ 0, 0 };
        static constexpr FramesAlignment         DEFAULT_DIRECTION{ FramesAlignment:: HORIZONTAL };

    public:

        Animation() = delete;
        Animation(float animation_duration, std::filesystem::path const& sprites_sheet_src, std::size_t frame_width, std::size_t frame_height, std::optional<std::size_t> n = std::nullopt, std::optional<FramesAlignment> direction = std::nullopt, std::optional<Geometry::Vector2D<int>> start_point = std::nullopt);
        Animation(Animation&&) = default;
        Animation(Animation const&) = default;
        Animation& operator=(Animation const&) = default;
        Animation& operator=(Animation&&) = default;

        void update(float dt);
        void reset() noexcept;

        bool is_finished() const noexcept;
        SurfaceView get_cur_frame() const noexcept;

        ~Animation() = default;

    private:

        inline void toggle_frame() noexcept;

    private:

        std::shared_ptr<std::vector<Surface>> frames;
        std::size_t cur_frame_index{ 0U };

        float frame_duration;
        float elapsed_duration{ 0.f };
    };
}