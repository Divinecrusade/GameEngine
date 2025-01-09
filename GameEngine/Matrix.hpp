#pragma once

#include <array>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <stdexcept>
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
        :
        data{ std::ranges::join_view(init_data) }
        { }

        constexpr Matrix(Matrix const&) = default;
        constexpr Matrix(Matrix&&)      = default;

        constexpr Matrix& operator=(Matrix const&) = default;
        constexpr Matrix& operator=(Matrix&&)      = default;

        constexpr ~Matrix() = default;

        template<std::ranges::input_range R>
        requires std::same_as<std::ranges::range_value_t<R>, T>
        Matrix (R&& init_data)
        {
            if (init_data.size() != M * N) 
                throw std::out_of_range{ "Attempt to initialize Matrix with range which size differs from matrix's number of elements"};
            std::ranges::copy(init_data, data.begin());
        }

    private:

        std::array<T, M * N> data{ };
    };
}