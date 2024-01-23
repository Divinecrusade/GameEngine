#pragma once

#include <chrono>


class FrameTimer final
{
public:

    FrameTimer()
    {
        last = std::chrono::steady_clock::now();
    }
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

    std::chrono::steady_clock::time_point last;
};