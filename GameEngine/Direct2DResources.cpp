#include "Direct2DFactory.hpp"

Direct2DResources::Direct2DResources(ID2D1Factory& D2DFactory, D2D1_HWND_RENDER_TARGET_PROPERTIES const& render_target_props)
{
    if (FAILED(S_OK | D2DFactory.CreateHwndRenderTarget(D2D1::RenderTargetProperties(), render_target_props, &render_target)))
        throw std::runtime_error{ "Failed to create D2D1 render target" };

    render_target->CreateSolidColorBrush(D2D1::ColorF{ D2D1::ColorF::LightSlateGray }, &light_slate_gray);
}

Direct2DResources::~Direct2DResources()
{
    safe_release(render_target);
    safe_release(light_slate_gray);
}
