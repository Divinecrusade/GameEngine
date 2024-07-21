#pragma once

#include "AnimationFrames.hpp"
#include <type_traits>


namespace GameEngine
{
    class AnimationFramesView final : public std::ranges::view_interface<AnimationFramesView>
    {
    public:

        AnimationFramesView() = default;
        AnimationFramesView(AnimationFrames const& frames)
        :
        begin_{ frames.cbegin() },
        end_  { frames.cend()   }
        { }

        AnimationFramesView(AnimationFramesView const&) = default;
        AnimationFramesView(AnimationFramesView&&) = default;

        AnimationFramesView& operator=(AnimationFramesView const&) = default;
        AnimationFramesView& operator=(AnimationFramesView&&) = default;

        ~AnimationFramesView() = default;

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
            return *(begin_ + i);
        }

    private:

        std::invoke_result_t<decltype(&AnimationFrames::cbegin), AnimationFrames> begin_{ };
        std::invoke_result_t<decltype(&AnimationFrames::cend),   AnimationFrames> end_{ };
    };
}