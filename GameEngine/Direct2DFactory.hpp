#pragma once

#include "Direct2DResources.hpp"

class Direct2DFactory
{
public:

    Direct2DFactory(HWND attached_window);
    ~Direct2DFactory();

    Direct2DResources get_resources() const;

private:
    
    HWND const attached_window;
    ID2D1Factory* D2DFactory{ nullptr };
};