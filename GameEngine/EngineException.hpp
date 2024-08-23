#pragma once

#include <stdexcept>
#include <concepts>
#include <memory>
#include <string>
#include <string_view>
#include <sstream>
#include <functional>
#include <type_traits>


namespace GameEngine::Auxiliary
{
    template<typename T>
    requires (std::is_default_constructible_v<T> == true && std::is_copy_assignable_v<T> == true && std::is_copy_constructible_v<T> == true)
    class EngineException : public std::exception
    {
    public:

        EngineException() noexcept(noexcept(T{ })) = default;
        EngineException(EngineException const&) noexcept(noexcept(T{ std::declval<T const&>() })) = default;
        EngineException(EngineException&&) noexcept = default;

        EngineException& operator=(EngineException const&) noexcept(noexcept(std::declval<T&>() = std::declval<T const&>())) = default;
        EngineException& operator=(EngineException&&) noexcept = default;

        virtual ~EngineException() noexcept = default;


        T const& get_error_code() const noexcept
        {
            return error_code;
        }

        std::wstring_view get_error_description() const noexcept
        {
            return error_description.get();
        }

        virtual std::wstring get_full_description() const noexcept
        {
            return get_description_builder().str();
        }

        virtual wchar_t const* get_exception_class_name() const noexcept
        {
            return L"GameEngine exception";
        }

    protected:

        EngineException(char const* msg, T const& error_code, std::shared_ptr<wchar_t const> error_description) noexcept(noexcept(T{ std::declval<T const&>() }))
        :
        exception{ msg },
        error_code{ error_code },
        error_description{ error_description }
        { }

        std::wostringstream get_description_builder() const noexcept
        {
            std::wostringstream wsout{ };

            wsout << L"[What happened]: " << what() << std::endl;
            wsout << L"[Error description]: " << error_description.get() << std::endl;

            return wsout;
        }

    private:

        T error_code{ };
        std::shared_ptr<wchar_t const> error_description{ nullptr };
    };
}