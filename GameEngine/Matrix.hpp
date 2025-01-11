#pragma once

#include <array>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <span>
#include <cassert>


namespace GameEngine::Geometry
{
    template<typename T>
    concept arithmetic_like = requires (T)
    {
        std::declval<T const&>() + std::declval<T const&>();
        std::declval<T const&>() - std::declval<T const&>();
        std::declval<T const&>() * std::declval<T const&>();
        std::declval<T const&>() / std::declval<T const&>();
        std::declval<T&>() = std::declval<T const&>();
    } && std::is_default_constructible_v<T> == true;
    static_assert(arithmetic_like<double>);
    static_assert(arithmetic_like<float>);
    static_assert(arithmetic_like<int>);
    static_assert(arithmetic_like<long>);
    static_assert(arithmetic_like<int32_t>);
    static_assert(arithmetic_like<uint32_t>);
    static_assert(arithmetic_like<int16_t>);
    static_assert(arithmetic_like<uint16_t>);
    static_assert(arithmetic_like<int8_t>);
    static_assert(arithmetic_like<uint8_t>);

    // M - number of rows
    // N - number of cols
    template<std::size_t M, std::size_t N = M, arithmetic_like T = double>
    requires (M != 0U && N != 0U)
    class Matrix final
    {
    public:

        static_assert(std::is_move_assignable_v<T>    == std::is_nothrow_move_assignable_v<T>);
        static_assert(std::is_move_constructible_v<T> == std::is_nothrow_move_constructible_v<T>);

        static constexpr std::size_t NUMBER_OF_ROWS{ M };
        static constexpr std::size_t NUMBER_OF_COLS{ N };

        constexpr Matrix() = default;

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...)) && std::is_move_constructible_v<T>
        constexpr explicit Matrix(Args&&... init_data) noexcept
        : 
        data{ { std::forward<Args>(init_data)... } } 
        { }

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...)) && !std::is_move_constructible_v<T>
        constexpr explicit Matrix(Args&&... init_data) noexcept(std::is_nothrow_copy_constructible<T>)
        :
        data{ { std::forward<Args>(init_data)... } }
        { }

        constexpr explicit Matrix(std::array<T, M * N> init_data) noexcept(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>)
        :
        data{ std::move(init_data) }
        { }
        constexpr explicit Matrix(std::array<std::array<T, N>, M> init_data) noexcept(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>)
        {
            for (std::size_t i{ 0U }; i != NUMBER_OF_ROWS; ++i)
            for (std::size_t j{ 0U }; j != NUMBER_OF_COLS; ++j)
                data[i * NUMBER_OF_COLS + j] = init_data[i][j];
        }

        constexpr Matrix(Matrix const&) = default;
        constexpr Matrix(Matrix&&)      = default;

        constexpr Matrix& operator=(Matrix const&) = default;
        constexpr Matrix& operator=(Matrix&&)      = default;

        constexpr ~Matrix() = default;

        template<std::ranges::input_range R>
        requires std::same_as<std::ranges::range_value_t<R>, T>
        Matrix (R&& init_data)
        {
            assert(("Input range must have size equal to number of elements in matrix", std::distance(init_data.begin(), init_data.end()) == NUMBER_OF_ROWS * NUMBER_OF_COLS));
            
            std::ranges::copy(init_data, data.begin());
        }

        std::span<T const, N> operator[](std::size_t row) const
        {
            assert(("Requested row's index must be lesser than number of matrix rows", row < NUMBER_OF_ROWS));

            auto const beg{ data.begin() + row * NUMBER_OF_COLS };
            return std::span<T const, N>{ beg, beg + NUMBER_OF_COLS };
        }

        std::span<T const, M* N> flattern() const noexcept
        {
            return data;
        }

        void swap_cols(std::size_t lhs, std::size_t rhs)
        {
            assert(("Requested column's index must be lesser than number of matrix columns", lhs < NUMBER_OF_COLS));
            assert(("Requested column's index must be lesser than number of matrix columns", rhs < NUMBER_OF_COLS));

            for (std::size_t i{ 0U }; i != NUMBER_OF_ROWS * NUMBER_OF_COLS; i += NUMBER_OF_COLS)
            {
                std::swap(data[lhs + i], data[rhs + i]);
            }
        }
        void swap_rows(std::size_t lhs, std::size_t rhs)
        {
            assert(("Requested row's index must be lesser than number of matrix rows", lhs < NUMBER_OF_ROWS));
            assert(("Requested row's index must be lesser than number of matrix rows", rhs < NUMBER_OF_ROWS));

            lhs = lhs * NUMBER_OF_COLS;
            rhs = rhs * NUMBER_OF_COLS;

            for (std::size_t i{ 0U }; i != NUMBER_OF_COLS; ++i)
            {
                std::swap(data[lhs + i], data[rhs + i]);
            }
        }

    private:

        std::array<T, NUMBER_OF_ROWS * NUMBER_OF_COLS> data{ };
    };
}
