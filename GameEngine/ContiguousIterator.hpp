#pragma once

#include <iterator>
#include <concepts>
#include <functional>


namespace GameEngine::Auxiliary
{
    template<class T, 
             class Container, 
             bool reversed = false>
    struct ContiguousIterator final
    {
    public:

        using value_type        = T;
        using element_type      = T;
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type   = std::ptrdiff_t;

        using pointer   = T*;
        using reference = T&;

    private:

        friend Container;

        ContiguousIterator(pointer data) noexcept
        :
        data{ data }
        { 
            static_assert(std::contiguous_iterator<ContiguousIterator<T, Container>>);
        }

    public:

        ContiguousIterator() noexcept = default;
        ContiguousIterator(ContiguousIterator const&) noexcept = default;
        ContiguousIterator(ContiguousIterator&&)      noexcept = default;

        template<class AnyContainer>
        ContiguousIterator(ContiguousIterator<T, AnyContainer, !reversed> const& it) noexcept
        :
        data{ it.data     + static_cast<difference_type>(reversed ? -1 : 1) }
        { }
        template<class AnyContainer>
        ContiguousIterator(ContiguousIterator<T, AnyContainer, !reversed>&& it_tmp)  noexcept
        :
        data{ it_tmp.data + static_cast<difference_type>(reversed ? -1 : 1) }
        { }

        ContiguousIterator& operator=(ContiguousIterator const&) noexcept = default;
        ContiguousIterator& operator=(ContiguousIterator&&)      noexcept = default;

        template<class AnyContainer>
        ContiguousIterator& operator=(ContiguousIterator<T, AnyContainer, !reversed> const& it) noexcept
        { 
            data = it.data    + static_cast<difference_type>(reversed ? -1 : 1);
        }
        template<class AnyContainer>
        ContiguousIterator& operator=(ContiguousIterator<T, AnyContainer, !reversed>&& it_tmp)  noexcept
        { 
            data = it_tmp.data + static_cast<difference_type>(reversed ? -1 : 1);
        }

        ~ContiguousIterator() = default;

        pointer   operator->() const noexcept
        {
            return &(*data);
        }
        reference operator*()  const noexcept
        {
            return *data;
        }
        reference operator[](difference_type delta) const noexcept
        {
            return *(*this + delta);
        }

        ContiguousIterator& operator+=(difference_type delta) noexcept
        {
            if constexpr(reversed) this->data -= delta;
            else                   this->data += delta;
            return *this;
        }
        ContiguousIterator& operator-=(difference_type delta) noexcept
        {
            if constexpr(reversed) this->data += delta;
            else                   this->data -= delta;
            return *this;
        }

        ContiguousIterator& operator++()    noexcept
        {
            return (*this += static_cast<difference_type>(1));
        }
        ContiguousIterator  operator++(int) noexcept
        {
            return std::exchange(*this, *this + static_cast<difference_type>(1));
        }

        ContiguousIterator& operator--()    noexcept
        {
            return (*this -= static_cast<difference_type>(1));
        }
        ContiguousIterator  operator--(int) noexcept
        {
            return std::exchange(*this, *this - static_cast<difference_type>(1));
        }

        friend ContiguousIterator operator+(ContiguousIterator const& lhs, difference_type rhs) noexcept
        {
            return ContiguousIterator{ lhs } += rhs;
        }
        friend ContiguousIterator operator+(difference_type lhs, ContiguousIterator const& rhs) noexcept
        {
            return ContiguousIterator{ rhs } += lhs;
        }

        friend ContiguousIterator operator-(ContiguousIterator const& lhs, difference_type rhs) noexcept
        {
            return ContiguousIterator{ lhs } -= rhs;
        }
        friend ContiguousIterator operator-(difference_type lhs, ContiguousIterator const& rhs) noexcept
        {
            return ContiguousIterator{ rhs } -= lhs;
        }
        friend difference_type operator-(ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return lhs.data - rhs.data;
        }

        friend bool operator==(ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return lhs.data == rhs.data;
        }
        friend bool operator!=(ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        friend bool operator< (ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return lhs.data < rhs.data;
        }
        friend bool operator> (ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return (lhs < rhs) && lhs != rhs;
        }
        friend bool operator<=(ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return lhs < rhs || lhs == rhs;
        }
        friend bool operator>=(ContiguousIterator const& lhs, ContiguousIterator const& rhs) noexcept
        {
            return lhs > rhs || lhs == rhs;
        }
    
    private:

        pointer data{ nullptr };
    };
}