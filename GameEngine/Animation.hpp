#pragma once

#include "Surface.hpp"
#include "SurfaceView.hpp"
#include "IGraphics2D.hpp"
#include "AnimationFramesView.hpp"


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
        Animation(AnimationFramesView frames, float animation_duration);
        Animation(Animation&&)      = default;
        Animation(Animation const&) = default;
        Animation& operator=(Animation const&) = default;
        Animation& operator=(Animation&&)      = default;

        void update(float dt);
        void reset() noexcept;

        bool is_finished() const noexcept;
        SurfaceView get_cur_frame() const noexcept;

        ~Animation() = default;

    private:

        inline void toggle_frame() noexcept;

    private:

        AnimationFramesView frames;
        std::size_t cur_frame_index{ 0U };

        float frame_duration;
        float elapsed_duration{ 0.f };
    };
}