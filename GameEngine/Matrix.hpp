#pragma once

#include <array>
#include <concepts>
#include <ranges>
#include <algorithm>


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

        Matrix() = default;

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...)) && std::is_move_constructible_v<T>
        explicit Matrix(Args&&... args) noexcept
        : 
        data{ { std::forward<Args>(args)... } } 
        { }

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...)) && !std::is_move_constructible_v<T>
        explicit Matrix(Args&&... args) noexcept(std::is_nothrow_copy_constructible<T>)
        :
        data{ { std::forward<Args>(args)... } }
        { }

        explicit Matrix(std::array<T, M * N> arr_data) noexcept(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>)
        :
        data{ std::move(arr_data) }
        { }

        explicit Matrix(std::array<std::array<T, N>, M> arr_data) noexcept(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>)
        :
        data{ std::ranges::join_view(arr_data) }
        { }

        Matrix(Matrix const&) = default;
        Matrix(Matrix&&)      = default;

        Matrix& operator=(Matrix const&) = default;
        Matrix& operator=(Matrix&&)      = default;

        ~Matrix() = default;

    private:

        std::array<T, M * N> data{ };
    };
}