#pragma once

#include "EngineException.hpp"

#include <Windows.h>
#include <comdef.h>


namespace GameEngine
{
    class Direct2dException : private _com_error, public Auxiliary::EngineException<HRESULT>
    {
    public:
        
        Direct2dException() = delete;
        Direct2dException(HRESULT error_code, char const* msg = nullptr)
        :
        _com_error{ error_code },
        EngineException{ msg, error_code, std::shared_ptr<wchar_t const>{ std::shared_ptr<void>{ nullptr }, this->ErrorMessage() } }
        { }
        Direct2dException(Direct2dException const&) noexcept = default;
        Direct2dException(Direct2dException&&) noexcept = default;

        Direct2dException& operator=(Direct2dException const&) noexcept = default;
        Direct2dException& operator=(Direct2dException&&) noexcept = default;

        ~Direct2dException() noexcept override = default;

        std::wstring get_full_description() const override
        {
            return (get_description_builder() << L"[Error code]: 0x" << std::hex << get_error_code()).str();
        }

        wchar_t const* get_exception_class_name() const noexcept override
        {
            return L"Direct2D exception";
        }
    };
}