#pragma once


#include "CellOfMatrix.hpp"
#include "ViewOfMatrix.hpp"

#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <span>


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
            const_iterator(T const* data, T const* base) noexcept
            :
            data{ data, (data - base) / N, (data - base) % N }
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
        void swap_cols(std::size_t lhs, std::size_t rhs)
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

    private:

        std::array<T, NUMBER_OF_ROWS * NUMBER_OF_COLS> data{ };
    };
}
