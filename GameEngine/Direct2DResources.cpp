#include "Direct2DResources.hpp"

Direct2DResources::Direct2DResources(HWND hWnd)
{
    if (FAILED(S_OK | D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory)))
        throw std::runtime_error{ "Failed to create D2D1 factory" };

    RECT render_area{ };
    GetClientRect(hWnd, &render_area);

    D2D1_SIZE_U const d2d1_size{ D2D1::SizeU(render_area.right - render_area.left, render_area.bottom - render_area.top) };

    if (FAILED(S_OK | D2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, d2d1_size), &render_target)))
        throw std::runtime_error{ "Failed to create D2D1 render target" };

    render_target->CreateSolidColorBrush(D2D1::ColorF{ D2D1::ColorF::LightSlateGray }, &light_slate_gray);
}

Direct2DResources::~Direct2DResources()
{
    safe_release(D2DFactory);
    safe_release(render_target);
    safe_release(light_slate_gray);
}
