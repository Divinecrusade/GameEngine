#include "GraphicsDirect2D.hpp"

GraphicsDirect2D::GraphicsDirect2D(HWND hWnd)
:
D2DFactory{ hWnd },
D2DResources{ }
{ }

// TODO
void GraphicsDirect2D::set_pixel(unsigned x, unsigned y, Color c)
{
    assert(composing_frame);
}

//TODO
Color GraphicsDirect2D::get_pixel(unsigned x, unsigned y) const
{
    assert(composing_frame);
    return Color::Azure;
}

void GraphicsDirect2D::draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w, Color c)
{
    assert(composing_frame);

    ID2D1SolidColorBrush* brush{ nullptr };
    D2DResources.get_renderer().CreateSolidColorBrush(c, &brush);
    D2DResources.get_renderer().DrawLine(D2D1_POINT_2F{static_cast<float>(x1), static_cast<float>(y1)},
                                         D2D1_POINT_2F{ static_cast<float>(x2), static_cast<float>(y2)}, brush, static_cast<float>(w));
    safe_release(brush);
}

void GraphicsDirect2D::fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c)
{
    assert(composing_frame);

    ID2D1SolidColorBrush* brush{ nullptr };
    D2DResources.get_renderer().CreateSolidColorBrush(c, &brush);
    D2DResources.get_renderer().FillRectangle(D2D1_RECT_F{static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2)}, brush);

    safe_release(brush);
}

void GraphicsDirect2D::draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c)
{
    assert(composing_frame);

    ID2D1SolidColorBrush* brush{ nullptr };
    D2DResources.get_renderer().CreateSolidColorBrush(c, &brush);
    D2DResources.get_renderer().DrawRectangle(D2D1_RECT_F{ static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2) }, brush);

    safe_release(brush);
}

void GraphicsDirect2D::begin_frame()
{
    assert(!composing_frame);

    D2DResources = { D2DFactory.get_resources() };
    composing_frame = true;
    D2DResources.get_renderer().BeginDraw();
}

void GraphicsDirect2D::end_frame()
{
    assert(composing_frame);

    composing_frame = false;
    D2DResources.get_renderer().EndDraw();
}

unsigned GraphicsDirect2D::get_screen_width() const noexcept
{
    return D2DFactory.get_render_area_size().right - D2DFactory.get_render_area_size().left;
}

unsigned GraphicsDirect2D::get_screen_height() const noexcept
{
    return D2DFactory.get_render_area_size().bottom - D2DFactory.get_render_area_size().top;
}
