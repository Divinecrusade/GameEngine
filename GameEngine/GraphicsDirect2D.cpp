#include "GraphicsDirect2D.hpp"


namespace GameEngine2D
{
    GraphicsDirect2D::GraphicsDirect2D(HWND hWnd)
    :
    d2d_factory{ hWnd }
    { }

    void GraphicsDirect2D::draw_line(Geometry::Vector2D<float> const& beg, Geometry::Vector2D<float> const& end, unsigned stroke_width, Colour c)
    {
        assert(composing_frame);

        d2d_factory.get_render_target().DrawLine(D2D1_POINT_2F{ beg.x, beg.y },
                                                 D2D1_POINT_2F{ end.x, end.y }, 
                                                 &d2d_factory.get_brush(c),
                                                 static_cast<float>(stroke_width));
    }

    void GraphicsDirect2D::fill_rectangle(Geometry::Rectangle2D<float> const& rect, Colour c)
    {
        assert(composing_frame);

        d2d_factory.get_render_target().FillRectangle(D2D1_RECT_F{ rect.left, rect.top, rect.right, rect.bottom }, 
                                                      &d2d_factory.get_brush(c));
    }

    void GraphicsDirect2D::draw_rectangle(Geometry::Rectangle2D<float> const& rect, unsigned stroke_width, Colour c)
    {
        assert(composing_frame);

        d2d_factory.get_render_target().DrawRectangle(D2D1_RECT_F{ rect.left, rect.top, rect.right, rect.bottom },
                                                      &d2d_factory.get_brush(c), static_cast<float>(stroke_width));
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
}