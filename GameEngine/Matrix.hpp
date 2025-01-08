#pragma once

#include <array>
#include <concepts>


// M - number of rows
// N - number of cols
template<std::size_t M, std::size_t N, class T = double>
requires (M != 0U && N != 0U && 
          std::is_arithmetic_v<T> == true && std::is_default_constructible_v<T> == true)
class Matrix final
{
public:

    Matrix() noexcept(std::is_nothrow_default_constructible_v<T>) = default;
    Matrix(Matrix const&) noexcept(noexcept(std::declval<T&>() = std::declval<T const&>())) = default;
    Matrix(Matrix&&)      noexcept = default;

    Matrix& operator=(Matrix const&) noexcept(std::is_nothrow_copy_constructible_v<Matrix>) = default;
    Matrix& operator=(Matrix&&)      noexcept = default;

    ~Matrix() noexcept = default;
};
