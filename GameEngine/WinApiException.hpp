#pragma once

#include <Windows.h>
#include <cassert>
#include <stdexcept>
#include <exception>
#include <errhandlingapi.h>
#include <WinUser.h>
#include <WinBase.h>
#include <memory>
#include <sstream>

#include "EngineException.hpp"


namespace GameEngine
{
    class WinApiException : public Auxiliary::EngineException<DWORD>
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
                    (LPTSTR)&description,
                    0,
                    nullptr
                )
            };
            assert(r);

            return description;
        }
        static void free_error_description(wchar_t const* ptr) noexcept
        {
            (void) LocalFree((HLOCAL)ptr);
        }

    public:

        WinApiException(char const* msg = nullptr)
        :
        EngineException{ msg, GetLastError(), alloc_error_description, free_error_description }
        { }
        WinApiException(WinApiException const&) noexcept = default;
        WinApiException(WinApiException&&) noexcept = default;

        WinApiException& operator=(WinApiException const&) noexcept = default;
        WinApiException& operator=(WinApiException&&) noexcept = default;

        ~WinApiException() noexcept = default;

        wchar_t const* get_exception_class_name() const noexcept override
        {
            return L"WinAPI exception";
        }
    };
}