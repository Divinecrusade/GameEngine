#include "Direct2DFactory.hpp"

Direct2DFactory::Direct2DFactory(HWND attached_window)
:
attached_window{ attached_window }
{
    if (FAILED(S_OK | D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory)))
        throw std::runtime_error{ "Failed to create D2D1 factory" };
}

Direct2DFactory::~Direct2DFactory()
{
    safe_release(D2DFactory);
}

Direct2DResources Direct2DFactory::get_resources() const
{
    RECT render_area{ };
    GetClientRect(attached_window, &render_area);
    D2D1_SIZE_U const d2d1_size{ D2D1::SizeU(render_area.right - render_area.left, render_area.bottom - render_area.top) };
    auto const render_properties { D2D1::HwndRenderTargetProperties(attached_window, d2d1_size) };

    return Direct2DResources{ *D2DFactory, render_properties };
}
