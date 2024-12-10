#pragma once

#include <KeyColor.hpp>


class PulsationEffect final
{
public:

    static constexpr float INIT_ELAPSED_DURATION{ 0.f };
    static constexpr float INIT_DELTA_SIGN{ +1.f };

    static constexpr float ITERATION_DURATION{ 1.5f };

public:

    constexpr PulsationEffect(GameEngine::Colour end_pulse_c, int end_pulse_a)
    :
    end_pulse_c{ end_pulse_c },
    end_pulse_a{ end_pulse_a }
    { }

    constexpr void update(float dt) noexcept
    {
        assert(dt > 0.f);

        elapsed_duration += dt * delta_sign;
        if (elapsed_duration > ITERATION_DURATION)
        {
            elapsed_duration = ITERATION_DURATION;
            delta_sign = -1.f;
        }
        else if (elapsed_duration < 0.f)
        {
            elapsed_duration = 0.f;
            delta_sign = +1.f;
        }
    }

    constexpr void reset() noexcept
    {
        elapsed_duration = INIT_ELAPSED_DURATION;
        delta_sign       = INIT_DELTA_SIGN;
    }

    constexpr GameEngine::Colour operator()(GameEngine::Colour c) const noexcept
    {
        return GameEngine::Colour::blend(c, end_pulse_c, static_cast<uint8_t>(end_pulse_a * (elapsed_duration / ITERATION_DURATION)));
    }

private:

    GameEngine::Colour const end_pulse_c;
    int const                end_pulse_a;

    float elapsed_duration{ INIT_ELAPSED_DURATION };
    float delta_sign{ INIT_DELTA_SIGN };
};