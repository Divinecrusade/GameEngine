#pragma once

#include "AnimationFrames.hpp"


namespace GameEngine
{
    class AnimationFramesView final : public std::ranges::view_interface<AnimationFramesView>
    {
    public:

        AnimationFramesView() noexcept = default;
        AnimationFramesView(AnimationFrames const& frames) noexcept
        :
        begin_{ frames.cbegin() },
        end_  { frames.cend()   }
        { }

        AnimationFramesView(AnimationFramesView const&) noexcept = default;
        AnimationFramesView(AnimationFramesView&&)      noexcept = default;

        AnimationFramesView& operator=(AnimationFramesView const&) noexcept = default;
        AnimationFramesView& operator=(AnimationFramesView&&)      noexcept = default;

        ~AnimationFramesView() noexcept = default;

        auto begin() const noexcept
        {
            return begin_;
        }

        auto end() const noexcept
        {
            return end_;
        }

        SurfaceView operator[](std::size_t i) const noexcept
        {
            assert(static_cast<ptrdiff_t>(i) < std::distance(begin_, end_));
            return *(begin_ + i);
        }

    private:

        std::invoke_result_t<decltype(&AnimationFrames::cbegin), AnimationFrames> begin_{ };
        std::invoke_result_t<decltype(&AnimationFrames::cend),   AnimationFrames> end_{ };
    };
}