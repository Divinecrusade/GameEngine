#pragma once

#include "Direct2DResources.hpp"

class Direct2DFactory
{
public:

    Direct2DFactory() = delete;
    Direct2DFactory(HWND attached_window);
    Direct2DFactory(Direct2DFactory const&) = delete;
    Direct2DFactory(Direct2DFactory&&) = delete;

    Direct2DFactory& operator=(Direct2DFactory const&) = delete;
    Direct2DFactory&& operator=(Direct2DFactory&&) = delete;

    ~Direct2DFactory();

    RECT get_render_area_size() const noexcept;
    Direct2DResources get_resources() const;

private:
    
    HWND const attached_window;
    ID2D1Factory* D2DFactory{ nullptr };
};