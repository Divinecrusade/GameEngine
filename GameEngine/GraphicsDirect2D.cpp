#include "GraphicsDirect2D.hpp"

GraphicsDirect2D::GraphicsDirect2D(HWND hWnd)
:
d2d_factory{ hWnd }
{ }

void GraphicsDirect2D::draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w, 
                                 unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(composing_frame);

    d2d_factory.get_render_target().DrawLine(D2D1_POINT_2F{ static_cast<float>(x1), static_cast<float>(y1) },
                                             D2D1_POINT_2F{ static_cast<float>(x2), static_cast<float>(y2) }, 
                                             &d2d_factory.get_brush(r, g, b, a),
                                             static_cast<float>(w));
}

void GraphicsDirect2D::fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, 
                                      unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(composing_frame);

    d2d_factory.get_render_target().FillRectangle(D2D1_RECT_F{ static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2) }, 
                                                  &d2d_factory.get_brush(r, g, b, a));
}

void GraphicsDirect2D::draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, 
                                      unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(composing_frame);

    d2d_factory.get_render_target().DrawRectangle(D2D1_RECT_F{ static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2) },
                                                  &d2d_factory.get_brush(r, g, b, a));
}

void GraphicsDirect2D::begin_frame()
{
    assert(!composing_frame);

    composing_frame = true;
    d2d_factory.get_render_target().BeginDraw();
}

void GraphicsDirect2D::end_frame()
{
    assert(composing_frame);

    composing_frame = false;
    d2d_factory.get_render_target().EndDraw();
    d2d_factory.free_resources();
}

unsigned GraphicsDirect2D::get_screen_width() const noexcept
{
    RECT const area_size{ d2d_factory.get_render_area_size() };
    return area_size.right - area_size.left;
}

unsigned GraphicsDirect2D::get_screen_height() const noexcept
{
    RECT const area_size{ d2d_factory.get_render_area_size() };
    return area_size.bottom - area_size.top;
}
