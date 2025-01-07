#pragma once

#include "KeyColor.hpp"
#include "ContiguousIterator.hpp"

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cwchar>
#include <tuple>
#include <array>
#include <cassert>


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

        const_iterator begin()  const noexcept;
        const_iterator end()    const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend()   const noexcept;

        reverse_iterator rbegin()              noexcept;
        reverse_iterator rend()                noexcept;
        const_reverse_iterator rbegin()  const noexcept;
        const_reverse_iterator rend()    const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend()   const noexcept;

        class RowView;
        RowView operator[](std::size_t i_row) const noexcept;

        class RowView final : public std::ranges::view_interface<RowView>
        {
            friend RowView Surface::operator[](std::size_t) const noexcept;
            RowView(Surface::const_iterator begin, Surface::const_iterator end)
            :
            begin_{ begin },
            end_{ end }
            { }

        public:

            RowView() = default;
            RowView(RowView const&) = default;
            RowView(RowView&&) = default;

            RowView& operator=(RowView const&) = default;
            RowView& operator=(RowView&&) = default;
            
            ~RowView() = default;

            Surface::const_iterator begin() const noexcept
            {
                return begin_;
            }

            Surface::const_iterator end() const noexcept
            {
                return end_;
            }

        private:

            Surface::const_iterator begin_{ }, end_{ };
        };

    public:

        static constexpr std::array<wchar_t const* const, 2U> SUPPORTED_EXTENSIONS{{ L".bmp", L".BMP" }};
        
        static constexpr std::size_t SUPPORTED_COLOUR_DEPTH   { 24U };
        static constexpr std::size_t SUPPORTED_COLOUR_CHANNEL {  8U };
        static constexpr std::size_t SUPPORTED_PIXEL_SIZE     { SUPPORTED_COLOUR_DEPTH / SUPPORTED_COLOUR_CHANNEL };

        class BMP_HANDLER final
        {
        public:

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

            std::ifstream fin;

            std::size_t const width;
            std::size_t const height;

            bool const reversed;

            std::streamoff const padding;
        };
        static BMP_HANDLER parse_img(std::filesystem::path const& img_src) noexcept;
        
    private:

        static std::tuple<std::unique_ptr<Colour[]>, std::size_t, std::size_t> read_img(BMP_HANDLER img);

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(std::unique_ptr<Colour[]> buffer, std::size_t n_rows, std::size_t n_cols);
        Surface(Surface const& other);
        Surface(class SurfaceView other);
        Surface(Surface&&) noexcept = default;

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