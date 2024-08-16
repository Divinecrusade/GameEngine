#pragma once

#include <Windows.h>
#include <cassert>
#include <stdexcept>
#include <exception>
#include <errhandlingapi.h>
#include <WinUser.h>
#include <WinBase.h>
#include <memory>


namespace GameEngine
{
    class WinApiException : public std::exception
    {
    private:

        static wchar_t const* alloc_error_description(DWORD error_code) noexcept
        {
            wchar_t const* description{ nullptr };

            DWORD const r 
            { 
                FormatMessageW
                (
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    nullptr,
                    error_code,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &description,
                    0,
                    nullptr
                ) 
            };
            assert(r);

            return description;
        }

    public:

        WinApiException() = delete;
        WinApiException(char const* message) noexcept
        :
        exception{ message },
        error_code{ GetLastError() },
        error_description{ alloc_error_description(error_code), [](wchar_t const* ptr) { (void) LocalFree((HLOCAL)ptr); }}
        { }
        WinApiException(WinApiException const&) noexcept = default;
        WinApiException(WinApiException&&) noexcept = default;

        WinApiException& operator=(WinApiException const&) noexcept = default;
        WinApiException& operator=(WinApiException&&) noexcept = default;

        ~WinApiException() noexcept = default;

        DWORD get_winapi_error_code() const noexcept
        {
            return error_code;
        }

        wchar_t const* get_winapi_error_description() const noexcept
        {
            return error_description.get();
        }

    private:

        DWORD error_code;
        std::shared_ptr<wchar_t const> error_description;
    };
}