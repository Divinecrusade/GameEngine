#pragma once

#pragma comment(lib, "d2d1.lib")

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <cassert>
#include <stdexcept>
#include <memory>
#include <type_traits>


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif // !HINST_THISCOMPONENT


template<class Interface>
concept Releasable =
requires (Interface & instance)
{
    instance.Release();
};

template<Releasable Interface>
__forceinline void safe_release(Interface*& realising_resource)
{
    if (realising_resource != nullptr)
    {
        realising_resource->Release();
        realising_resource = nullptr;
    }
}


class Direct2DFactory;

class Direct2DResources final
{
public:

    Direct2DResources(ID2D1Factory& D2DFactory, D2D1_HWND_RENDER_TARGET_PROPERTIES const& render_target_props);
    ~Direct2DResources();

private:

    ID2D1HwndRenderTarget* render_target{ nullptr };
    ID2D1SolidColorBrush* light_slate_gray{ nullptr };
};