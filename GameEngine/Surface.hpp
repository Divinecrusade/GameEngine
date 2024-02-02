#pragma once

#include "ISurface.hpp"

#include <filesystem>


namespace GameEngine
{
    class Surface : Interfaces::ISurface
    {
    public:

        static constexpr char const* const SUPPORTED_EXTENSION{ "bmp" };
        static constexpr int SUPPORTED_PIXEL_SIZE{ 4 };

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(Surface const&) = delete;
        Surface(Surface&&) = delete;

        Surface& operator=(Surface const&) = delete;
        Surface& operator=(Surface&&) = delete;

        virtual ~Surface() noexcept;

        virtual void draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma) const override;
        virtual std::vector<std::reference_wrapper<Colour>> get_bitmap() const override;

        virtual int get_width() const noexcept override;
        virtual int get_height() const noexcept override;

    private:
        
        int width;
        int height;

        Colour* buffer;
    };
}