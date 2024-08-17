#pragma once

#include "Surface.hpp"


namespace GameEngine
{
    class SurfaceView final : public std::ranges::view_interface<SurfaceView>
    {
    public:

        SurfaceView() noexcept = default;
        SurfaceView(Surface const& srf) noexcept
        :
        begin_ { srf.begin() },
        end_   { srf.end()   },
        width_ { srf.get_width()  },
        height_{ srf.get_height() }
        { }

        SurfaceView(SurfaceView const&) noexcept = default;
        SurfaceView(SurfaceView&&) noexcept = default;

        SurfaceView& operator=(SurfaceView const&) noexcept = default;
        SurfaceView& operator=(SurfaceView&&) noexcept = default;

        ~SurfaceView() noexcept = default;

        Surface::const_iterator begin() const noexcept
        {
            return begin_;
        }

        Surface::const_iterator end() const noexcept
        {
            return end_;
        }

        std::size_t get_width() const noexcept
        {
            return width_;
        }

        std::size_t get_height() const noexcept
        {
            return height_;
        }

    private:

        Surface::const_iterator begin_{ }, end_{ };
        std::size_t width_{ 0U }, height_{ 0U };
    };
}