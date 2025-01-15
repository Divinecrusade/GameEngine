#pragma once


#include "CellOfMatrix.hpp"
#include "ViewOfMatrix.hpp"
#include "Auxiliry.hpp"

#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <span>
#include <functional>


namespace GameEngine::Geometry::Matrices
{
    template<typename T>
    concept arithmetic_like = requires (T)
    {
        std::declval<T const&>() + std::declval<T const&>();
        std::declval<T const&>() - std::declval<T const&>();
        std::declval<T const&>() * std::declval<T const&>();
        std::declval<T const&>() / std::declval<T const&>();
        std::declval<T&>() = std::declval<T const&>();
        std::declval<T const&>() == std::declval<T const&>();
        std::declval<T const&>() != std::declval<T const&>();
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

    public:

        class const_iterator;
        constexpr const_iterator begin() const noexcept
        {
            return const_iterator{ std::to_address(data.begin()), std::to_address(data.begin()) };
        }
        constexpr const_iterator end()   const noexcept
        {
            return const_iterator{ std::to_address(data.end()), std::to_address(data.begin()) };
        }

        class const_iterator final
        {
        public:

            using value_type   = CellOfMatrix<T>;
            using element_type = CellOfMatrix<T>;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            using pointer   = value_type const*;
            using reference = value_type const&;

        private:

            friend constexpr const_iterator Matrix::begin() const noexcept;
            friend constexpr const_iterator Matrix::end()   const noexcept;
            const_iterator(T const* init_data, T const* base) noexcept
            :
            data{ init_data, (init_data - base) / N, (init_data - base) % N }
            { }

        public:

            constexpr const_iterator() noexcept = default;
            constexpr const_iterator(const_iterator const&) noexcept = default;
            constexpr const_iterator(const_iterator&&)      noexcept = default;

            constexpr const_iterator& operator=(const_iterator const&) noexcept = default;
            constexpr const_iterator& operator=(const_iterator&&)      noexcept = default;

            constexpr ~const_iterator() noexcept = default;


            constexpr pointer   operator->() const noexcept
            {
                return &data;
            }
            constexpr reference operator*()  const noexcept
            {
                return data;
            }


            constexpr const_iterator& operator+=(difference_type delta) noexcept
            {
                std::size_t const d_row{ std::abs(delta) / N };
                std::size_t const d_col{ std::abs(delta) % N };

                if (delta > 0)
                {
                    data.set_row_index(data.get_row_index() + d_row + (data.get_col_index() + d_col >= N));
                    data.set_col_index((data.get_col_index() + d_col) % N);
                }
                else
                {
                    assert(("Iterator exceeds matrix's rows during decreasing", d_row + (data.get_col_index() < d_col) <= data.get_row_index()));
                    data.set_row_index(data.get_row_index() - d_row - (data.get_col_index() < d_col));
                    data.set_col_index(data.get_col_index() + N * (data.get_col_index() < d_col) - d_col);
                }
                data.move_pointer(delta);

                return *this;
            }
            constexpr const_iterator& operator-=(difference_type delta) noexcept
            {
                return (*this += -delta);
            }

            constexpr const_iterator& operator++()    noexcept
            {
                return (*this += static_cast<difference_type>(1));
            }
            constexpr const_iterator  operator++(int) noexcept
            {
                return std::exchange(*this, *this + static_cast<difference_type>(1));
            }

            constexpr const_iterator& operator--()    noexcept
            {
                return (*this -= static_cast<difference_type>(1));
            }
            constexpr const_iterator  operator--(int) noexcept
            {
                return std::exchange(*this, *this - static_cast<difference_type>(1));
            }

            friend constexpr const_iterator operator+(const_iterator const& lhs, difference_type rhs) noexcept
            {
                return const_iterator{ lhs } += rhs;
            }
            friend constexpr const_iterator operator+(difference_type lhs, const_iterator const& rhs) noexcept
            {
                return const_iterator{ rhs } += lhs;
            }

            friend constexpr const_iterator operator-(const_iterator const& lhs, difference_type rhs) noexcept
            {
                return const_iterator{ lhs } -= rhs;
            }
            friend constexpr const_iterator operator-(difference_type lhs, const_iterator const& rhs) noexcept
            {
                return const_iterator{ rhs } -= lhs;
            }
            friend constexpr difference_type operator-(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return lhs.data - rhs.data;
            }

            friend constexpr bool operator==(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return &lhs.data.get_data() == &rhs.data.get_data();
            }
            friend constexpr bool operator!=(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return !(lhs == rhs);
            }

            friend constexpr bool operator< (const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return &lhs.data.get_data() < &rhs.data.get_data();
            }
            friend constexpr bool operator> (const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return (lhs < rhs) && lhs != rhs;
            }
            friend constexpr bool operator<=(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return lhs < rhs || lhs == rhs;
            }
            friend constexpr bool operator>=(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return lhs > rhs || lhs == rhs;
            }

        private:
            
            value_type data{ };
        };
        static_assert(std::bidirectional_iterator<const_iterator>);

    public:

        constexpr Matrix() = default;

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...) && std::is_move_constructible_v<T>)
        constexpr explicit Matrix(Args&&... init_data) noexcept
        : 
        data{ { std::forward<Args>(init_data)... } } 
        { }

        template<typename... Args>
        requires (sizeof...(Args) == M * N && (std::is_same_v<Args, T> && ...) && !std::is_move_constructible_v<T>)
        constexpr explicit Matrix(Args&&... init_data) noexcept(std::is_nothrow_copy_constructible_v<T>)
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

        std::span<T const, N> operator[](std::size_t row) const noexcept
        {
            assert(("Requested row's index must be lesser than number of matrix rows", row < NUMBER_OF_ROWS));

            auto const beg{ data.begin() + row * NUMBER_OF_COLS };
            return std::span<T const, N>{ beg, beg + NUMBER_OF_COLS };
        }

        std::span<T const, M* N> flattern() const noexcept
        {
            return data;
        }

        void swap_rows(std::size_t lhs, std::size_t rhs) noexcept
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
        void swap_cols(std::size_t lhs, std::size_t rhs) noexcept
        {
            assert(("Requested column's index must be lesser than number of matrix columns", lhs < NUMBER_OF_COLS));
            assert(("Requested column's index must be lesser than number of matrix columns", rhs < NUMBER_OF_COLS));

            for (std::size_t i{ 0U }; i != NUMBER_OF_ROWS * NUMBER_OF_COLS; i += NUMBER_OF_COLS)
            {
                std::swap(data[lhs + i], data[rhs + i]);
            }
        }

        constexpr auto get_row(std::size_t i) const noexcept
        {
            assert(("Requested row's index must not exceed the number of matrix's rows", i < M));
            return get_row_impl(i, std::make_index_sequence<NUMBER_OF_COLS>{});
        }   
        constexpr auto get_col(std::size_t i) const noexcept
        {
            assert(("Requested column's index must not exceed the number of matrix's columns", i < N));
            return get_col_impl(i, std::make_index_sequence<NUMBER_OF_ROWS>{});
        }

        constexpr Matrix<N, M, T> get_transponsed() const noexcept(std::is_nothrow_assignable_v<T, T>)
        {
            std::array<T, N* M> tmp_data{ };

            for (std::size_t i{ 0U }; i != N; ++i)
            for (std::size_t j{ 0U }; j != M; ++j)
            {
                tmp_data[i * M + j] = data[j * N + i];
            }

            return Matrix<N, M, T>{ std::move(tmp_data) };
        }

        Matrix& sum_rows(std::size_t from, std::size_t to, T const& from_k) noexcept(noexcept(std::declval<T const&>()* std::declval<T const&>()) && noexcept(std::declval<T const&>() + std::declval<T const&>()) && std::is_nothrow_assignable_v<T, T>)
        {
            assert(("Index of requested row must not exceed the size of matrix", from < NUMBER_OF_ROWS));
            assert(("Index of requested row must not exceed the size of matrix", to < NUMBER_OF_ROWS));
            assert(("It is forbidden to add row with itself", from != to));
            
            for (std::size_t i{ 0U }; i != NUMBER_OF_COLS; ++i)
            {
                data[to * NUMBER_OF_COLS + i] = data[to * NUMBER_OF_COLS + i] + data[from * NUMBER_OF_COLS + i] * from_k;
            }

            return *this;
        }
        Matrix& sum_cols(std::size_t from, std::size_t to, T const& from_k) noexcept(noexcept(std::declval<T const&>()* std::declval<T const&>()) && noexcept(std::declval<T const&>() + std::declval<T const&>()) && std::is_nothrow_assignable_v<T, T>)
        {
            assert(("Index of requested column must not exceed the size of matrix", from < NUMBER_OF_COLS));
            assert(("Index of requested column must not exceed the size of matrix", to < NUMBER_OF_COLS));
            assert(("It is forbidden to add column with itself", from != to));

            for (std::size_t i{ 0U }; i != NUMBER_OF_ROWS; ++i)
            {
                data[i * NUMBER_OF_COLS + to] = data[i * NUMBER_OF_COLS + to] + data[i * NUMBER_OF_COLS + from] * from_k;
            }

            return *this;
        }

        Matrix& mul_row(std::size_t i, T const& k) noexcept(noexcept(std::declval<T const&>()* std::declval<T const&>()) && std::is_nothrow_assignable_v<T, T>)
        {
            assert(("Index of requested row must not exceed the size of matrix", i < NUMBER_OF_ROWS));
            for (std::size_t j{ 0U }; j != NUMBER_OF_COLS; ++j)
            {
                data[i * NUMBER_OF_COLS + j] = data[i * NUMBER_OF_COLS + j] * k;
            }

            return *this;
        }
        Matrix& mul_col(std::size_t j, T const& k) noexcept(noexcept(std::declval<T const&>()* std::declval<T const&>()) && std::is_nothrow_assignable_v<T, T>)
        {
            assert(("Index of requested column must not exceed the size of matrix", j < NUMBER_OF_COLS));
            for (std::size_t i{ 0U }; i != NUMBER_OF_ROWS; ++i)
            {
                data[i * NUMBER_OF_COLS + j] = data[i * NUMBER_OF_COLS + j] * k;
            }

            return *this;
        }

    private:

        template<std::size_t... Indices>
        constexpr auto get_row_impl(std::size_t i, std::index_sequence<Indices...>) const noexcept
        {
            return ViewOfMatrix<N, T>{ TypeOfMatrixView::ROW, i, &data[i * NUMBER_OF_COLS + Indices]... };
        }

        template<std::size_t... Indices>
        constexpr auto get_col_impl(std::size_t i, std::index_sequence<Indices...>) const noexcept
        {
            return ViewOfMatrix<M, T>{ TypeOfMatrixView::COLUMN, i, &data[i + Indices * NUMBER_OF_COLS]... };
        }

    public:

        friend constexpr bool operator==(Matrix const& lhs, Matrix const& rhs) noexcept
        {
            if constexpr (std::floating_point<T>)
            {
                for (std::size_t i{ 0U }; i != lhs.data.size(); ++i)
                {
                    if (!Auxiliry::is_equal_with_precision(lhs.data[i], rhs.data[i]))
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return lhs.data == rhs.data;
            }
        }

        friend constexpr bool operator!=(Matrix const& lhs, Matrix const& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        friend constexpr Matrix operator+(Matrix const& lhs, Matrix const& rhs) noexcept(noexcept(std::declval<T const&>() + std::declval<T const&>()))
        {
            std::array<T, M * N> sum_result{ };

            std::ranges::transform(lhs.data, rhs.data, sum_result.begin(), std::plus<T>{});

            return Matrix{ std::move(sum_result) };
        }
        friend constexpr Matrix operator-(Matrix const& lhs, Matrix const& rhs) noexcept(noexcept(std::declval<T const&>() - std::declval<T const&>()))
        {
            std::array<T, M* N> sub_result{ };

            std::ranges::transform(lhs.data, rhs.data, sub_result.begin(), std::minus<T>{});

            return Matrix{ std::move(sub_result) };
        }
        friend constexpr Matrix operator*(Matrix const& lhs, T const& rhs) noexcept(noexcept(std::declval<T const&>() * std::declval<T const&>()))
        {
            std::array<T, M * N> mul_result{ };

            std::ranges::transform(lhs.data, mul_result.begin(), [&rhs](auto const& val) noexcept { return rhs * val; });

            return Matrix{ std::move(mul_result) };
        }
        friend constexpr Matrix operator*(T const& lhs, Matrix const& rhs) noexcept(noexcept(std::declval<T const&>() * std::declval<T const&>()))
        {
            return rhs * lhs;
        }
        friend constexpr Matrix operator/(Matrix const& lhs, T const& rhs) noexcept(noexcept(std::declval<T const&>() / std::declval<T const&>()))
        {
            std::array<T, M* N> mul_result{ };

            std::ranges::transform(lhs.data, mul_result.begin(), [&rhs](auto const& val) noexcept { return val / rhs; });

            return Matrix{ std::move(mul_result) };
        }


        Matrix& operator+=(Matrix const& rhs) noexcept(noexcept(std::declval<T const&>() + std::declval<T const&>()))
        {
            std::ranges::transform(data, rhs.data, data.begin(), std::plus<T>{});
            return *this;
        }
        Matrix& operator-=(Matrix const& rhs) noexcept(noexcept(std::declval<T const&>() - std::declval<T const&>()))
        {
            std::ranges::transform(data, rhs.data, data.begin(), std::minus<T>{});
            return *this;
        }
        Matrix& operator*=(T const& rhs) noexcept(noexcept(std::declval<T const&>() * std::declval<T const&>()))
        {
            std::ranges::transform(data, data.begin(), [&rhs](auto const& val) noexcept { return rhs * val; });
            return *this;
        }
        Matrix& operator/=(T const& rhs) noexcept(noexcept(std::declval<T const&>() / std::declval<T const&>()))
        {
            std::ranges::transform(data, data.begin(), [&rhs](auto const& val) noexcept { return val / rhs; });
            return *this;
        }
        Matrix& operator*=(Matrix<N, M, T> const& rhs)
        {
            *this = (*this * rhs);
            return *this;
        }

    private:

        std::array<T, NUMBER_OF_ROWS * NUMBER_OF_COLS> data{ };
    };

    template<std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, typename T>
    requires (M1 != M2 || N1 != N2)
    constexpr bool operator==(Matrix<M1, N1, T> const&, Matrix<M2, N2, T> const&) noexcept
    {
        return false;
    }

    template<std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, typename T>
    requires (M1 != M2 || N1 != N2)
    constexpr bool operator!=(Matrix<M1, N1, T> const&, Matrix<M2, N2, T> const&) noexcept
    {
        return true;
    }

    template<std::size_t M, std::size_t N, std::size_t K, typename T>
    Matrix<M, K, T> operator*(Matrix<M, N, T> const& lhs, Matrix<N, K, T> const& rhs) noexcept(noexcept(std::declval<T const&>()* std::declval<T const&>()) && noexcept(std::declval<T const&>() + std::declval<T const&>()))
    {
        std::array<T, M* K> mul_result{ };

        auto const lhs_data{ lhs.flattern() };
        auto const rhs_data{ rhs.flattern() };

        for (std::size_t left_row{ 0U }; left_row != M; ++left_row)
        for (std::size_t right_col{ 0U }; right_col != K; ++right_col)
        for (std::size_t i{ 0U }; i != N; ++i)
            mul_result[left_row * K + right_col] = mul_result[left_row * K + right_col] + lhs_data[left_row * N + i] * rhs_data[i * K + right_col];

        return Matrix<M, K, T>{ std::move(mul_result) };
    }

    template<std::size_t S, typename T>
    auto get_vector_row(ViewOfMatrix<S, T> const& init_data) noexcept
    {
        std::array<T, S> tmp{ };
        
        std::ranges::copy(init_data, tmp.begin());

        return Matrix<1U, S, T>{ std::move(tmp) };
    }

    template<std::size_t S, typename T>
    auto get_vector_col(ViewOfMatrix<S, T> const& init_data) noexcept
    {
        std::array<T, S> tmp{ };

        std::ranges::copy(init_data, tmp.begin());

        return Matrix<S, 1U, T>{ std::move(tmp) };
    }

    template<std::size_t M, std::size_t N, std::size_t K, typename T>
    Matrix<M, N + K, T> expand_by_row(Matrix<M, N, T> const& lhs, Matrix<M, K, T> const& rhs) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        std::array<T, M * (N + K)> concat_result{ };

        auto const lhs_data{ lhs.flattern() };
        auto const rhs_data{ rhs.flattern() };

        for (std::size_t i{ 0U }; i != M; ++i)
        for (std::size_t j{ 0U }; j != N + K; ++j)
        {
            concat_result[i * (N + K) + j] = (j >= N ? rhs_data[i * K + (j - N)] : lhs_data[i * N + j]);
        }

        return Matrix<M, N + K, T>{ std::move(concat_result) };
    }

    template<std::size_t M, std::size_t N, std::size_t K, typename T>
    Matrix<M + K, N, T> expand_by_col(Matrix<M, N, T> const& lhs, Matrix<K, N, T> const& rhs) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        std::array<T, (M + K) * N> concat_result{ };

        auto const lhs_data{ lhs.flattern() };
        auto const rhs_data{ rhs.flattern() };

        for (std::size_t i{ 0U }; i != M + K; ++i)
        for (std::size_t j{ 0U }; j != N; ++j)
        {
            concat_result[i * N + j] = (i >= M ? rhs_data[(i - M) * N + j] : lhs_data[i * N + j]);
        }

        return Matrix<M + K, N, T>{ std::move(concat_result) };
    }

    template<std::size_t I1, std::size_t J1, std::size_t I2, std::size_t J2, std::size_t M, std::size_t N, typename T>
    requires (I1 <= I2 && J1 <= J2 && I2 < M && J2 < N)
    Matrix<I2 - I1 + 1U, J2 - J1 + 1U, T> get_from(Matrix<M, N, T> const& m) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        std::array<T, (I2 - I1 + 1U) * (J2 - J1 + 1U)> split_result{ };

        auto const m_data{ m.flattern() };

        for (std::size_t i{ 0U }; i != (I2 - I1 + 1U); ++i)
        for (std::size_t j{ 0U }; j != (J2 - J1 + 1U); ++j)
        {
            split_result[i * (J2 - J1 + 1U) + j] = m_data[(I1 + i) * N + (J1 + j)];
        }

        return Matrix<I2 - I1 + 1U, J2 - J1 + 1U, T>{ std::move(split_result) };
    }
    
    template<std::size_t M, typename T = double>
    constexpr Matrix<M, M, T> get_identity() noexcept
    {
        std::array<T, M* M> data{ };

        for (std::size_t i = 0; i < M; ++i)
        {
            data[i * M + i] = T{ 1 };
        }

        return Matrix<M, M, T>{ std::move(data) };
    }

    template<std::size_t M, std::size_t N, typename T>
    constexpr Matrix<N, M, T> get_transponsed(Matrix<M, N, T> const& m) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        return m.get_transponsed();
    }

    template<std::size_t M, typename T>
    requires (!std::floating_point<T>)
    T get_determinant(Matrix<M, M, T> m)
    {
        static constexpr T ZERO{ 0 };
        static constexpr T ONE{ 1 };
        static constexpr T MINUS{ -1 };

        T D{ ONE };
        
        for (std::size_t I{ 0U }; I != m.NUMBER_OF_ROWS - 1U; ++I)
        {
            if (m[I][I] == ZERO)
            {
                std::size_t i{ };
                for (i = I + 1U; i != m.NUMBER_OF_ROWS; ++i)
                {
                    if (m[i][I] != ZERO) break;
                }

                if (i == m.NUMBER_OF_ROWS) throw std::runtime_error{ "Matrix is singular" };

                m.swap_rows(I, i);

                D = D * MINUS;
            }

            for (std::size_t i{ I + 1U }; i != m.NUMBER_OF_ROWS; ++i)
            {
                m.sum_rows(I, i, MINUS * (m[i][I] / m[I][I]));
            }
        }

        for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
            D = D * m[i][i];

        return D;
    }

    template<std::size_t M, typename T>
    requires (std::floating_point<T>)
    T get_determinant(Matrix<M, M, T> m)
    {
        static constexpr T ZERO { 0 };
        static constexpr T ONE  { 1 };
        static constexpr T MINUS{ -1 };

        T D{ ONE };

        for (std::size_t I{ 0U }; I != m.NUMBER_OF_ROWS - 1U; ++I)
        {
            if (Auxiliry::is_equal_with_precision(m[I][I], ZERO))
            {
                std::size_t i{ };
                for (i = I + 1U; i != m.NUMBER_OF_ROWS; ++i)
                {
                    if (!Auxiliry::is_equal_with_precision(m[i][I], ZERO)) break;
                }

                if (i == m.NUMBER_OF_ROWS) throw std::runtime_error{ "Matrix is singular" };

                m.swap_rows(I, i);

                D = D * MINUS;
            }

            for (std::size_t i{ I + 1U }; i != m.NUMBER_OF_ROWS; ++i)
            {
                m.sum_rows(I, i, MINUS *(m[i][I] / m[I][I]));
            }
        }

        for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
            D = D * m[i][i];

        return D;
    }

    template<std::size_t M, typename T>
    requires (!std::floating_point<T>)
    Matrix<M, M, T> get_inversed(Matrix<M, M, T> const& m)
    {
        static constexpr T ZERO{ 0 };
        static constexpr T ONE{ 1 };
        static constexpr T MINUS{ -1 };

        auto expanded{ expand_by_row(m, get_identity<M, T>()) };

        for (std::size_t I{ 0U }; I != m.NUMBER_OF_ROWS - 1U; ++I)
        {
            if (expanded[I][I] == ZERO)
            {
                std::size_t i{ };
                for (i = I + 1U; i != m.NUMBER_OF_ROWS; ++i)
                {
                    if (expanded[i][I] != ZERO) break;
                }

                if (i == expanded.NUMBER_OF_ROWS) throw std::runtime_error{ "Matrix is singular" };

                expanded.swap_rows(I, i);
            }

            expanded.mul_row(I, ONE / expanded[I][I]);

            for (std::size_t i{ I + 1U }; i != m.NUMBER_OF_ROWS; ++i)
            {
                expanded.sum_rows(I, i, MINUS * (expanded[i][I] / expanded[I][I]));
            }
        }

        expanded.mul_row(M - 1U, ONE / expanded[M - 1U][M - 1U]);

        for (std::size_t I{ m.NUMBER_OF_ROWS - 1U }; I != 0U; --I)
        for (std::size_t i{ I }; i != 0U; --i)
        {
            expanded.sum_rows(I, i - 1U, MINUS * (expanded[i - 1U][I] / expanded[I][I]));
        }

        return get_from<0U, M, M - 1U, M + M - 1U>(expanded);
    }

    template<std::size_t M, typename T>
    requires (std::floating_point<T>)
    Matrix<M, M, T> get_inversed(Matrix<M, M, T> const& m)
    {
        static constexpr T ZERO{ 0 };
        static constexpr T ONE{ 1 };
        static constexpr T MINUS{ -1 };

        auto expanded{ expand_by_row(m, get_identity<M, T>()) };

        for (std::size_t I{ 0U }; I != m.NUMBER_OF_ROWS - 1U; ++I)
        {
            if (Auxiliry::is_equal_with_precision(expanded[I][I], ZERO))
            {
                std::size_t i{ };
                for (i = I + 1U; i != m.NUMBER_OF_ROWS; ++i)
                {
                    if (Auxiliry::is_equal_with_precision(expanded[i][I], ZERO)) break;
                }

                if (i == expanded.NUMBER_OF_ROWS) throw std::runtime_error{ "Matrix is singular" };

                expanded.swap_rows(I, i);
            }

            expanded.mul_row(I, ONE / expanded[I][I]);

            for (std::size_t i{ I + 1U }; i != m.NUMBER_OF_ROWS; ++i)
            {
                expanded.sum_rows(I, i, MINUS * (expanded[i][I] / expanded[I][I]));
            }
        }

        expanded.mul_row(M - 1U, ONE / expanded[M - 1U][M - 1U]);

        for (std::size_t I{ m.NUMBER_OF_ROWS - 1U }; I != 0U; --I)
        for (std::size_t i{ I }; i != 0U; --i)
        {
            expanded.sum_rows(I, i - 1U, MINUS * (expanded[i - 1U][I] / expanded[I][I]));
        }
    
        return get_from<0U, M, M - 1U, M + M - 1U>(expanded);
    }

    template<std::size_t M, std::size_t N, typename T>
    Matrix<M - 1U, N, T> remove_row(Matrix<M, N, T> const& m, std::size_t I) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        assert(("Requested row's index must not exceed the number of matrix's rows", I < M));
        std::array<T, (M - 1U)* N> tmp_data{ };

        auto const data{ m.flattern() };
        auto it{ tmp_data.begin() };
        for (std::size_t i{ 0U }; i != M; ++i)
        {
            if (i == I) continue;

            for (std::size_t j{ 0U }; j != N; ++j)
            {
                *it = data[i * N + j];

                ++it;
            }
        }

        return Matrix<M - 1U, N, T>{ std::move(tmp_data) };
    }
    
    template<std::size_t M, std::size_t N, typename T>
    Matrix<M, N - 1U, T> remove_col(Matrix<M, N, T> const& m, std::size_t J) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        assert(("Requested column's index must not exceed the number of matrix's columns", J < N));
        std::array<T, M* (N - 1U)> tmp_data{ };

        auto const data{ m.flattern() };
        auto it{ tmp_data.begin() };
        for (std::size_t i{ 0U }; i != M; ++i)
            for (std::size_t j{ 0U }; j != N; ++j)
            {
                if (j == J) continue;

                *it = data[i * N + j];

                ++it;
            }

        return Matrix<M, N - 1U, T>{ std::move(tmp_data) };
    }
    
    template<std::size_t M, std::size_t N, typename T>
    Matrix<M - 1U, N - 1U, T> remove_row_and_col(Matrix<M, N, T> const& m, std::size_t I, std::size_t J) noexcept(std::is_nothrow_assignable_v<T, T>)
    {
        assert(("Requested row's index must not exceed the number of matrix's rows", I < M));
        assert(("Requested column's index must not exceed the number of matrix's columns", J < N));
        std::array<T, (M - 1U)* (N - 1U)> tmp_data{ };

        auto const data{ m.flattern() };
        auto it{ tmp_data.begin() };
        for (std::size_t i{ 0U }; i != M; ++i)
        {
            if (i == I) continue;

            for (std::size_t j{ 0U }; j != N; ++j)
            {
                if (j == J) continue;

                *it = data[i * N + j];

                ++it;
            }
        }

        return Matrix<M - 1U, N - 1U, T>{ std::move(tmp_data) };
    }
}
