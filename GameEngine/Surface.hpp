#pragma once

#include "KeyColor.hpp"
#include "ContiguousIterator.hpp"

#include <Windows.h>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cwchar>
#include <tuple>


namespace GameEngine
{
    class Surface final
    {
    public:

        using iterator       = GameEngine::Auxiliary::ContiguousIterator<Colour, Surface>;
        using const_iterator = GameEngine::Auxiliary::ContiguousIterator<Colour const, Surface>;

        using reverse_iterator       = GameEngine::Auxiliary::ContiguousIterator<Colour, Surface, true>;
        using const_reverse_iterator = GameEngine::Auxiliary::ContiguousIterator<Colour const, Surface, true>;


        iterator begin() noexcept;
        iterator end()   noexcept;
        std::pair<iterator const, iterator const> operator[](std::size_t i_row) noexcept;

        const_iterator begin()  const noexcept;
        const_iterator end()    const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend()   const noexcept;
        std::pair<const_iterator const, const_iterator const> operator[](std::size_t i_row) const noexcept;

        reverse_iterator rbegin()              noexcept;
        reverse_iterator rend()                noexcept;
        const_reverse_iterator rbegin()  const noexcept;
        const_reverse_iterator rend()    const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend()   const noexcept;

    public:

        static constexpr wchar_t const* const SUPPORTED_EXTENSIONS[]{ L".bmp", L".BMP" };
        static constexpr std::size_t          SUPPORTED_COLOUR_DEPTH   { 24U };
        static constexpr std::size_t          SUPPORTED_COLOUR_CHANNEL {  8U };
        static constexpr std::size_t          SUPPORTED_PIXEL_SIZE     { SUPPORTED_COLOUR_DEPTH / SUPPORTED_COLOUR_CHANNEL };

        class BMP_HANDLER final
        {
        public:

            BMP_HANDLER() noexcept = default;
            BMP_HANDLER(std::ifstream&& fin, std::size_t width, std::size_t height, bool reversed, std::streamoff padding)
            :
            fin{ std::move(fin) },
            width{ width },
            height{ height },
            reversed{ reversed },
            padding{ padding }
            { }
            BMP_HANDLER(BMP_HANDLER const&) noexcept = default;
            BMP_HANDLER(BMP_HANDLER&&) noexcept = default;

            BMP_HANDLER& operator=(BMP_HANDLER const&) noexcept = default;
            BMP_HANDLER& operator=(BMP_HANDLER&&) noexcept = default;

            ~BMP_HANDLER() noexcept = default;

            std::ifstream& get_stream() noexcept
            {
                return fin;
            }
            std::ifstream const& get_stream() const noexcept
            {
                return fin;
            }

            std::size_t get_width() const noexcept
            {
                return width;
            }
            std::size_t get_height() const noexcept
            {
                return height;
            }

            bool is_reversed() const noexcept
            {
                return reversed;
            }

            std::streamoff get_padding() const noexcept
            {
                return padding;
            }

            constexpr std::size_t get_pixel_size() const noexcept
            {
                return Surface::SUPPORTED_PIXEL_SIZE;
            }

            std::size_t get_pixels_table_y_start() const noexcept
            {
                return (reversed ? 0U : height - 1U);
            }
            std::size_t get_pixels_table_y_end() const noexcept
            {
                return (reversed ? height : 0U);
            }

            int get_pixels_table_dy() const noexcept
            {
                return (reversed ? 1 : -1);
            }

        private:

            std::ifstream fin{ };

            std::size_t width{ };
            std::size_t height{ };

            bool reversed{ };

            std::streamoff padding{ };
        };
        static BMP_HANDLER parse_img(std::filesystem::path const& img_src);
        
    private:

        static std::tuple<std::unique_ptr<Colour[]>, std::size_t, std::size_t> read_img(BMP_HANDLER&& img);

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(std::unique_ptr<Colour[]> buffer, std::size_t n_rows, std::size_t n_cols);
        Surface(Surface const& other);
        Surface(class SurfaceView other);
        Surface(Surface&& other_tmp) noexcept;

        Surface& operator=(Surface const&) = delete;
        Surface& operator=(Surface&&)      = delete;

        ~Surface() noexcept = default;

        std::size_t get_width()  const noexcept;
        std::size_t get_height() const noexcept;

    private:

        std::unique_ptr<Colour[]> buffer;

        std::size_t const n_rows;
        std::size_t const n_cols;
    };
}