#pragma once

#include <array>
#include <optional>
#include <cassert>
#include <iterator>
#include <concepts>


namespace GameEngine::Geometry::Matrices
{
    enum class TypeOfMatrixView
    {
        COLUMN,
        ROW
    };

    template<std::size_t N, typename T>
    class ViewOfMatrix final
    {
    public:

        static constexpr std::size_t NUMBER_OF_ELEMENTS{ N };

    public:

        class const_iterator final
        {
        public:

            using value_type   = T;
            using element_type = T;
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            using pointer   = T const*;
            using reference = T const&;

        public:

            constexpr const_iterator() noexcept = default;
            constexpr explicit const_iterator(const T* const* init_data)
            :
            data{ init_data }
            { }
            constexpr const_iterator(const_iterator const&) noexcept = default;
            constexpr const_iterator(const_iterator&&)      noexcept = default;

            constexpr const_iterator& operator=(const_iterator const&) noexcept = default;
            constexpr const_iterator& operator=(const_iterator&&)      noexcept = default;

            constexpr ~const_iterator() noexcept = default;


            constexpr pointer   operator->() const noexcept
            {
                return &(**data);
            }
            constexpr reference operator*()  const noexcept
            {
                return **data;
            }
            constexpr reference operator[](difference_type delta) const noexcept
            {
                return *(*this + delta);
            }

            constexpr const_iterator& operator+=(difference_type delta) noexcept
            {
                this->data += delta;
                return *this;
            }
            constexpr const_iterator& operator-=(difference_type delta) noexcept
            {
                this->data -= delta;
                return *this;
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
                return lhs.data == rhs.data;
            }
            friend constexpr bool operator!=(const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return !(lhs == rhs);
            }

            friend constexpr bool operator< (const_iterator const& lhs, const_iterator const& rhs) noexcept
            {
                return lhs.data < rhs.data;
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

            const T* const* data{ nullptr };
        };
        static_assert(std::random_access_iterator<const_iterator>);

    public:

        constexpr ViewOfMatrix() = default;

        template<typename... Args>
        requires (sizeof...(Args) == N && (std::is_same_v<Args, T const*> && ...))
        constexpr ViewOfMatrix(TypeOfMatrixView init_type, std::size_t init_index, Args&&... init_data) noexcept
        :
        type{ init_type },
        index{ init_index },
        data{ { std::forward<Args>(init_data)... } }
        { }

        constexpr ViewOfMatrix(ViewOfMatrix const&) = default;
        constexpr ViewOfMatrix(ViewOfMatrix&&)      = default;

        constexpr ViewOfMatrix& operator=(ViewOfMatrix const&) = default;
        constexpr ViewOfMatrix& operator=(ViewOfMatrix&&)      = default;

        constexpr ~ViewOfMatrix() = default;


        constexpr std::optional<TypeOfMatrixView> get_type()  const noexcept
        {
            return type;
        }
        constexpr std::optional<std::size_t>      get_index() const noexcept
        {
            return index;
        }


        constexpr T const& operator[](std::size_t i) const noexcept
        {
            assert(("Operand of square bracket operator must not exceed view's range", i < NUMBER_OF_ELEMENTS));
            return *data[i];
        }


        const_iterator begin() const noexcept
        {
            return const_iterator{ std::to_address(data.begin()) };
        }
        const_iterator end()   const noexcept
        {
            return const_iterator{ std::to_address(data.end()) };
        }

    private:

        std::optional<TypeOfMatrixView> type { std::nullopt };
        std::optional<std::size_t>      index{ std::nullopt };

        std::array<T const*, N> data{ };
    };
}