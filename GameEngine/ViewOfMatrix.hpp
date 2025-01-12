#pragma once

#include <array>
#include <optional>
#include <cassert>


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


        constexpr std::optional<TypeOfMatrixView> get_type() const noexcept
        {
            return type;
        }
        constexpr std::optional<std::size_t>      get_index() const noexcept
        {
            return index;
        }


        T const& operator[](std::size_t i) const noexcept
        {
            assert(("Operand of square bracket operator must not exceed view's range", i < N));
            return *data[i];
        }

    private:

        std::optional<TypeOfMatrixView> type { std::nullopt };
        std::optional<std::size_t>      index{ std::nullopt };

        std::array<T const*, N> data{ };
    };
}