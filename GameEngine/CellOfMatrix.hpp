#pragma once

#include <utility>
#include <concepts>
#include <type_traits>
#include <cassert>


namespace GameEngine::Geometry::Matrices
{
    template<typename T>
    class CellOfMatrix final
    {
    public:

        constexpr CellOfMatrix() = default;
        constexpr CellOfMatrix(T const* init_data, std::size_t init_row, std::size_t init_col) noexcept
        :
        data{ init_data },
        row{ init_row },
        col{ init_col }
        { }
        constexpr CellOfMatrix(CellOfMatrix const&) = default;
        constexpr CellOfMatrix(CellOfMatrix&&)      = default;

        constexpr CellOfMatrix& operator=(CellOfMatrix const&) = default;
        constexpr CellOfMatrix& operator=(CellOfMatrix&&)      = default;
        
        constexpr ~CellOfMatrix() = default;


        constexpr T const& get_data() const noexcept
        {
            return *data;
        }

        constexpr std::size_t get_row_index() const noexcept
        {
            return row;
        }
        constexpr std::size_t get_col_index() const noexcept
        {
            return col;
        }

        constexpr std::pair<std::size_t, std::size_t> get_indices() const noexcept
        {
            return std::make_pair(get_row_index(), get_col_index());
        }

        constexpr void set_col_index(std::size_t new_col) noexcept
        {
            col = new_col;
        }
        constexpr void set_row_index(std::size_t new_row) noexcept
        {
            row = new_row;
        }

        constexpr void move_pointer(std::ptrdiff_t delta)
        {
            data += delta;
        }

    private:
    
        T const* data{ nullptr };

        std::size_t row{ 0U };
        std::size_t col{ 0U };
    };
}
