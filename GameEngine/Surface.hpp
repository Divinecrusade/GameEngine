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
        static constexpr int                  SUPPORTED_COLOUR_DEPTHS[]{ 24 };
        static constexpr int                  SUPPORTED_COLOUR_CHANNEL {  8 };

        static constexpr std::size_t IMG_FIN        { 0U };
        static constexpr std::size_t IMG_WIDTH      { 1U };
        static constexpr std::size_t IMG_HEIGHT     { 2U };
        static constexpr std::size_t IMG_REVERSED{ 3U };
        static constexpr std::size_t IMG_PADDING    { 4U };
        static constexpr std::size_t IMG_PIXEL_SIZE { 5U };
        static std::tuple<std::ifstream, std::size_t const, std::size_t const, bool const, std::streamoff const, int const> parse_img(std::filesystem::path const& img_src);
        
        static constexpr std::size_t PIXEL_TABLE_INFO_Y_START{ 0U };
        static constexpr std::size_t PIXEL_TABLE_INFO_Y_END  { 1U };
        static constexpr std::size_t PIXEL_TABLE_INFO_DY     { 2U };
        static std::tuple<std::size_t const, std::size_t const, int const> get_pixel_table_info(bool reversed, std::size_t height);

    private:

        static std::tuple<std::unique_ptr<Colour[]>, std::size_t, std::size_t> read_img(std::tuple<std::ifstream, std::size_t const, std::size_t const, bool const, std::streamoff const, int const>&& img);

    private:

        static constexpr int BYTE_SIZE{ 8 };
        static constexpr int SUPPORTED_PIXEL_SIZES[]
        {
            SUPPORTED_COLOUR_DEPTHS[0U] / BYTE_SIZE
        };

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(std::unique_ptr<Colour[]> buffer, std::size_t n_rows, std::size_t n_cols);
        Surface(Surface const& other);
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