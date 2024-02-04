#pragma once

#include "ISurface.hpp"

#include <filesystem>


namespace GameEngine
{
    class Surface : Interfaces::ISurface
    {
    public:

        static constexpr wchar_t const* const    SUPPORTED_EXTENSIONS[]{ L"bmp", L"BMP" };
        static constexpr int                     SUPPORTED_COLOUR_DEPTHS[]{ 24, 32 };

    private:

        static constexpr int SUPPORTED_PIXEL_SIZES[]{ SUPPORTED_COLOUR_DEPTHS[0] / 8, SUPPORTED_COLOUR_DEPTHS[1] / 8 };

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(Surface const&) = delete;
        Surface(Surface&&) = delete;

        Surface& operator=(Surface const&) = delete;
        Surface& operator=(Surface&&) = delete;

        virtual ~Surface() noexcept;

        virtual void draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma) const override;
        virtual std::vector<std::reference_wrapper<Colour const>> get_bitmap() const override;

        virtual int get_width() const noexcept override;
        virtual int get_height() const noexcept override;

    private:
        
        int width;
        int height;

        Colour* buffer;
    };
}