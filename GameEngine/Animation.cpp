#include "Animation.hpp"


namespace GameEngine
{
    Animation::Animation(AnimationFramesView frames, float animation_duration)
    :
    frames{ frames },
    frame_duration{ animation_duration / frames.size() }
    { }

    void Animation::update(float dt)
    {
        elapsed_duration += dt;
        while (elapsed_duration >= frame_duration)
        {
            toggle_frame();
            elapsed_duration -= frame_duration;
        }
    }

    bool Animation::is_finished() const noexcept
    {
        return cur_frame_index == frames.size();
    }

    void Animation::reset() noexcept
    {
        elapsed_duration = 0.f;
        cur_frame_index = 0U;
    }

    SurfaceView GameEngine::Animation::get_cur_frame() const noexcept
    {
        assert(cur_frame_index < frames.size());
        return frames[cur_frame_index];
    }

    inline void Animation::toggle_frame() noexcept
    {
        if (is_finished()) return;
        ++cur_frame_index;
    }
}