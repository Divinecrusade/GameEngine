#pragma once

#include "KeyColor.hpp"

#include <Windows.h>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cwchar>
#include <tuple>
#include <iterator>
#include <concepts>


namespace GameEngine
{
    class Surface final
    {
    public:

        struct iterator;

        iterator begin() noexcept;
        iterator end()   noexcept;
        iterator begin() const noexcept;
        iterator end()   const noexcept;
        std::pair<iterator const, iterator const> operator[](size_t i_row) noexcept;
        
        struct iterator final
        {
        private:

            friend iterator Surface::begin() noexcept;
            friend iterator Surface::end()   noexcept;
            friend iterator Surface::begin() const noexcept;
            friend iterator Surface::end()   const noexcept;
            friend std::pair<iterator const, iterator const> Surface::operator[](size_t i_row) noexcept;
            iterator(Colour* buffer) noexcept;

        public:

            using value_type        = Colour;
            using element_type      = Colour;
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type   = ptrdiff_t;

            using pointer           = Colour*;
            using reference         = Colour&;

            iterator() noexcept                = default;
            iterator(iterator const&) noexcept = default;
            iterator(iterator&&)      noexcept = default;

            iterator& operator=(iterator const&) noexcept = default;
            iterator& operator=(iterator&&)      noexcept = default;

            ~iterator() noexcept = default;

            pointer   operator->() const noexcept;
            reference operator*()  const noexcept;
            reference operator[](difference_type delta) const noexcept;

            iterator& operator+=(difference_type delta) noexcept;
            iterator& operator-=(difference_type delta) noexcept;

            iterator& operator++()    noexcept;
            iterator  operator++(int) noexcept;

            iterator& operator--()    noexcept;
            iterator  operator--(int) noexcept;

            friend iterator operator+(iterator const& lhs, difference_type rhs) noexcept;
            friend iterator operator+(difference_type lhs, iterator const& rhs) noexcept;

            friend iterator operator-(iterator const& lhs, difference_type rhs) noexcept;
            friend iterator operator-(difference_type lhs, iterator const& rhs) noexcept;
            friend difference_type operator-(iterator const& lhs, iterator const& rhs) noexcept;

            friend bool operator==(iterator const& lhs, iterator const& rhs) noexcept;
            friend bool operator!=(iterator const& lhs, iterator const& rhs) noexcept;

            friend bool operator< (iterator const& lhs, iterator const& rhs) noexcept;
            friend bool operator> (iterator const& lhs, iterator const& rhs) noexcept;
            friend bool operator<=(iterator const& lhs, iterator const& rhs) noexcept;
            friend bool operator>=(iterator const& lhs, iterator const& rhs) noexcept;

        private:

            Colour* data{ nullptr };
        };
        static_assert(std::contiguous_iterator<iterator>);

    public:

        static constexpr wchar_t const* const    SUPPORTED_EXTENSIONS[]{ L".bmp", L".BMP" };
        static constexpr int                     SUPPORTED_COLOUR_DEPTHS[]{ 24 };
        static constexpr int                     SUPPORTED_COLOUR_CHANNEL{ 8 };

        static std::tuple<std::ifstream, size_t const, size_t const, bool const, std::streamoff const, int const> parse_img(std::filesystem::path const& img_src);
        
    private:

        static std::tuple<std::unique_ptr<Colour[]>, size_t, size_t> read_img(std::tuple<std::ifstream, size_t const, size_t const, bool const, std::streamoff const, int const>&& img);

    private:

        static constexpr int                     BYTE_SIZE{ 8 };
        static constexpr int                     SUPPORTED_PIXEL_SIZES[]
        {
            SUPPORTED_COLOUR_DEPTHS[0U] / BYTE_SIZE
        };

    public:

        Surface() = delete;
        Surface(std::filesystem::path const& img_src);
        Surface(std::unique_ptr<Colour[]> buffer, size_t n_rows, size_t n_cols);
        Surface(Surface const& other);
        Surface(Surface&& other_tmp) noexcept;

        Surface& operator=(Surface const&) = delete;
        Surface& operator=(Surface&&) = delete;

        ~Surface() noexcept = default;

        size_t get_width()  const noexcept;
        size_t get_height() const noexcept;

    private:

        std::unique_ptr<Colour[]> buffer;

        size_t const n_rows;
        size_t const n_cols;
    };
}