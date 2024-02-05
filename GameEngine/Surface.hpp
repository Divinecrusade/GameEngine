#pragma once

#include "ISurface.hpp"

#include <Windows.h>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cwchar>


namespace GameEngine
{
    class Surface : public Interfaces::ISurface
    {
    public:

        static constexpr wchar_t const* const    SUPPORTED_EXTENSIONS[]{ L".bmp", L".BMP" };
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

        virtual ~Surface() noexcept = default;

        virtual std::shared_ptr<Colour const[]> get_pixels() const override;

        virtual size_t get_width() const noexcept override;
        virtual size_t get_height() const noexcept override;

    private:
        
        int width;
        int height;

        std::shared_ptr<Colour[]> buffer;
    };
}