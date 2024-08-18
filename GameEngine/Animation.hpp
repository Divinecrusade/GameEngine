#pragma once

#include "Surface.hpp"
#include "SurfaceView.hpp"
#include "IGraphics2D.hpp"
#include "AnimationFramesView.hpp"


namespace GameEngine
{
    class Animation final
    {
    public:

        Animation() = delete;
        Animation(AnimationFramesView frames, float animation_duration) noexcept;
        Animation(Animation&&)      noexcept = default;
        Animation(Animation const&) noexcept = default;
        Animation& operator=(Animation const&) noexcept = default;
        Animation& operator=(Animation&&)      noexcept = default;

        void update(float dt) noexcept;
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