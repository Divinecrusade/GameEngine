#pragma once

#include "SurfaceView.hpp"
#include "Vector2D.hpp"


namespace GameEngine
{   
    class AnimationFrames final
    {
    public:

        enum class FramesAlignment
        {
            HORIZONTAL, VERTICAL
        };

        static constexpr Geometry::Vector2D<int> DEFAULT_START_POINT{ 0, 0 };
        static constexpr FramesAlignment         DEFAULT_DIRECTION{ FramesAlignment::HORIZONTAL };

    public:

        AnimationFrames() = delete;
        AnimationFrames(std::filesystem::path const& sprites_sheet_src, std::size_t frame_width, std::size_t frame_height, std::optional<std::size_t> n = std::nullopt, std::optional<FramesAlignment> direction = std::nullopt, std::optional<Geometry::Vector2D<int>> start_point = std::nullopt);
        AnimationFrames(AnimationFrames&&)      noexcept = default;
        AnimationFrames(AnimationFrames const&) noexcept = default;
        AnimationFrames& operator=(AnimationFrames const&) = delete;
        AnimationFrames& operator=(AnimationFrames&&)      = delete;

        ~AnimationFrames() noexcept = default;

        std::size_t get_n_frames() const noexcept
        {
            return frames.size();
        }

        SurfaceView operator[](std::size_t i) const noexcept
        {
            assert(i < frames.size());
            return frames[i];
        }

        auto begin() const noexcept
        {
            return frames.begin();
        }
        auto end()   const noexcept
        {
            return frames.end();
        }

        auto begin() noexcept
        {
            return frames.begin();
        }
        auto end()   noexcept
        {
            return frames.end();
        }

        auto cbegin() const noexcept
        {
            return frames.cbegin();
        }
        auto cend()   const noexcept
        {
            return frames.cend();
        }

        auto rbegin() const noexcept
        {
            return frames.rbegin();
        }
        auto rend()   const noexcept
        {
            return frames.rend();
        }

        auto rbegin() noexcept
        {
            return frames.rbegin();
        }
        auto rend()   noexcept
        {
            return frames.rend();
        }

        auto crbegin() const noexcept
        {
            return frames.crbegin();
        }
        auto crend()   const noexcept
        {
            return frames.crend();
        }

    private:

        std::vector<GameEngine::Surface> frames;
    };
}