#pragma once

#include <chrono>


namespace GameEngine
{
    class FrameTimer final
    {
    public:

        FrameTimer() = default;
        FrameTimer(FrameTimer const&) = delete;
        FrameTimer(FrameTimer&&) = delete;

        FrameTimer& operator=(FrameTimer const&) = delete;
        FrameTimer& operator=(FrameTimer&&) = delete;

        float mark()
        {
            auto const old{ last };
            last = std::chrono::steady_clock::now();
            std::chrono::duration<float> const frameTime{ last - old };

            return frameTime.count();
        }

    private:

        std::chrono::steady_clock::time_point last{ std::chrono::steady_clock::now() };
    };
}
