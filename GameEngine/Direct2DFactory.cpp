#include "Direct2DFactory.hpp"

Direct2DFactory::Direct2DFactory(HWND attached_window)
:
attached_window{ attached_window }
{
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory)))
        throw std::runtime_error{ "Failed to create D2D1 factory" };
}

Direct2DFactory::~Direct2DFactory()
{
    safe_release(d2d_factory);
}

RECT Direct2DFactory::get_render_area_size() const noexcept
{
    RECT render_area{ };
    GetClientRect(attached_window, &render_area);

    return render_area;
}

ID2D1HwndRenderTarget& Direct2DFactory::get_render_target()
{   
    if (!render_target)
    {
        RECT const render_area{ get_render_area_size() };
        D2D1_SIZE_U const d2d1_size{ D2D1::SizeU(render_area.right - render_area.left, render_area.bottom - render_area.top) };
        D2D1_HWND_RENDER_TARGET_PROPERTIES const render_properties{ D2D1::HwndRenderTargetProperties(attached_window, d2d1_size) };
        if (FAILED(d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), render_properties, &render_target)))
            throw std::runtime_error{ "Failed to create D2D1 render target" };
    }
    
    return *render_target;
}

ID2D1SolidColorBrush& Direct2DFactory::get_brush(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(render_target);

    KeyColor const key{ r, g, b, a };
    if (!brushes.contains(key))
    {
        ID2D1SolidColorBrush* tmp{ nullptr };
        D2D1::ColorF const c{ get_color(r, g, b, a) };
        render_target->CreateSolidColorBrush(c, &tmp);
        brushes.insert({ key, tmp });
    }

    return *brushes[key];
}

void Direct2DFactory::free_resources()
{
    safe_release(render_target);
    std::for_each(brushes.begin(), brushes.end(), [](auto& pair) { safe_release(pair.second); } );
    brushes.clear();
}

KeyColor::KeyColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
{
    colors[0U] = r;
    colors[1U] = g;
    colors[2U] = b;
    colors[3U] = a;
}

KeyColor::KeyColor(KeyColor const& c) noexcept
{
    rgba = c.rgba;
}

KeyColor::KeyColor(KeyColor&& c) noexcept
{
    std::swap(rgba, c.rgba);
}

KeyColor& KeyColor::operator=(KeyColor const& c) noexcept
{
    rgba = c.rgba;

    return *this;
}

KeyColor& KeyColor::operator=(KeyColor&& c) noexcept
{
    std::swap(rgba, c.rgba);

    return *this;
}

uint32_t KeyColor::get_encoded() const noexcept
{
    return rgba;
}

bool operator==(KeyColor const& lhs, KeyColor const& rhs)
{
    return lhs.get_encoded() == rhs.get_encoded();
}