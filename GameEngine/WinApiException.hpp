#pragma once

#include "EngineException.hpp"

#include <Windows.h>
#include <cassert>
#include <errhandlingapi.h>


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
            std::ignore = LocalFree((HLOCAL)ptr);
        }

    public:

        WinApiException(char const* msg = nullptr) noexcept
        :
        WinApiException{ msg, GetLastError() }
        { }
        WinApiException(WinApiException const&) noexcept = default;
        WinApiException(WinApiException&&) noexcept = default;

        WinApiException& operator=(WinApiException const&) noexcept = default;
        WinApiException& operator=(WinApiException&&) noexcept = default;

        ~WinApiException() noexcept override = default;

        std::wstring get_full_description() const noexcept override
        {
            try
            {
                return (get_description_builder() << L"[Error code]: 0x" << std::hex << get_error_code()).str();
            }
            catch (...)
            {
                return L"Got something unexpected";
            }
        }

        wchar_t const* get_exception_class_name() const noexcept override
        {
            return L"WinAPI exception";
        }
    
    private:

        WinApiException(char const* msg, DWORD error_code) noexcept
        :
        EngineException{ msg, error_code, std::shared_ptr<wchar_t const>{ alloc_error_description(error_code), [](wchar_t const* ptr) noexcept { std::ignore = LocalFree((HLOCAL) ptr); } }}
        { }
    };
}
