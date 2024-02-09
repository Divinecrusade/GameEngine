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

        struct IMG_info
        {
            std::ifstream fin{ };

            int width{ };
            int height{ };

            int y_start{ };
            int y_end{ };
            int dy{ };

            int padding{ };

            int pixel_size{ };
        };

        static constexpr wchar_t const* const    SUPPORTED_EXTENSIONS[]{ L".bmp", L".BMP" };
        static constexpr int                     SUPPORTED_COLOUR_DEPTHS[]{ 24, 32 };

        static inline IMG_info go_to_pixels(std::filesystem::path const& img_src);

    private:

        static constexpr int SUPPORTED_PIXEL_SIZES[]{ SUPPORTED_COLOUR_DEPTHS[0] / 8, SUPPORTED_COLOUR_DEPTHS[1] / 8 };

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(size_t width, size_t height, std::shared_ptr<Colour const[]> buffer);
        Surface(Surface const& srf);
        Surface(Surface&& tmp) noexcept;

        Surface& operator=(Surface const& srf);
        Surface& operator=(Surface&& tmp) noexcept;

        virtual ~Surface() noexcept = default;

        virtual std::shared_ptr<Colour const[]> get_pixels() const override;

        virtual size_t get_width() const noexcept override;
        virtual size_t get_height() const noexcept override;

    private:

        void swap(Surface&& tmp) noexcept;

    private:
        
        size_t width{ 0U };
        size_t height{ 0U };

        std::shared_ptr<Colour const[]> buffer;
    };
}