#include "Direct2DFactory.hpp"

Direct2DResources::Direct2DResources(ID2D1Factory& D2DFactory, D2D1_HWND_RENDER_TARGET_PROPERTIES const& render_target_props)
{
    if (FAILED(S_OK | D2DFactory.CreateHwndRenderTarget(D2D1::RenderTargetProperties(), render_target_props, &render_target)))
        throw std::runtime_error{ "Failed to create D2D1 render target" };
}

Direct2DResources::Direct2DResources(Direct2DResources&& resources) noexcept
{
    swap(std::move(resources));
}

Direct2DResources& Direct2DResources::operator=(Direct2DResources&& resources) noexcept
{
    swap(std::move(resources));

    return *this;
}

Direct2DResources::~Direct2DResources()
{
    safe_release(render_target);
}

ID2D1HwndRenderTarget& Direct2DResources::get_renderer() const noexcept
{
    return *render_target;
}

void Direct2DResources::swap(Direct2DResources&& resources) noexcept
{
    std::swap(this->render_target, resources.render_target);
}
