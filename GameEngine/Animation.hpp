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
        
        enum class FramesAlignment
        {
            HORIZONTAL, VERTICAL
        };
        static std::vector<Surface> get_frames_from_sprites_sheet(std::filesystem::path const& sprites_sheet_src, size_t frame_width, size_t frame_height, size_t n = UNKNOWN_N, FramesAlignment direction = FramesAlignment::HORIZONTAL, Geometry::Vector2D<int> start_point = LEFT_TOP);

    public:

        Animation() = delete;
        Animation(std::shared_ptr<std::vector<Surface> const> const& frames, float frame_duration);
        Animation(Animation const&)            = default;
        Animation& operator=(Animation const&) = default;
        Animation& operator=(Animation&&)      = default;

        void update(float dt);
        void reset() noexcept;

        bool is_finished() const noexcept;
        Surface const& get_cur_frame() const noexcept;

        ~Animation() noexcept = default;

    private:

        inline void toggle_frame() noexcept;

    private:

        std::shared_ptr<std::vector<Surface> const> frames;
        size_t cur_frame_index{ 0U };

        float frame_duration;
        float elapsed_duration{ 0.f };
    };
}