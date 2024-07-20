#pragma once

#include "Surface.hpp"


namespace GameEngine
{
    class SurfaceView final : public std::ranges::view_interface<SurfaceView>
    {
    public:

        SurfaceView() = default;
        SurfaceView(Surface const& srf)
        :
        begin_ { srf.begin() },
        end_   { srf.end()   },
        width_ { srf.get_width()  },
        height_{ srf.get_height() }
        { }

        SurfaceView(SurfaceView const&) = default;
        SurfaceView(SurfaceView&&) = default;

        SurfaceView& operator=(SurfaceView const&) = default;
        SurfaceView& operator=(SurfaceView&&) = default;

        ~SurfaceView() = default;

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