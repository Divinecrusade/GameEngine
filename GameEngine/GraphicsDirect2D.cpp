#include "GraphicsDirect2D.hpp"


namespace GameEngine
{
    GraphicsDirect2D::GraphicsDirect2D(HWND hWnd)
    :
    d2d_factory{ hWnd }
    { }

    void GraphicsDirect2D::draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(beg.x >= 0);
        assert(beg.y >= 0);
        assert(end.y >= 0);
        assert(end.x >= 0);
        assert(stroke_width > 0);

        D2D1_POINT_2F const p1{ get_dips_from_pixels(beg.x), get_dips_from_pixels(beg.y) };
        D2D1_POINT_2F const p2{ get_dips_from_pixels(end.x), get_dips_from_pixels(end.y) };

        d2d_factory.get_render_target().DrawLine(p1, p2, &d2d_factory.get_brush(c), static_cast<float>(stroke_width));
    }

    void GraphicsDirect2D::fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0);
        assert(rect.right >= 0);
        assert(rect.bottom >= 0);
        assert(rect.top >= 0);

        D2D1_RECT_F const DIPs
        {
            get_dips_from_pixels(rect.left),
            get_dips_from_pixels(rect.top),
            get_dips_from_pixels(rect.right),
            get_dips_from_pixels(rect.bottom)
        };

        d2d_factory.get_render_target().FillRectangle(DIPs, &d2d_factory.get_brush(c));
    }

    void GraphicsDirect2D::draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0);
        assert(rect.right >= 0);
        assert(rect.bottom >= 0);
        assert(rect.top >= 0);

        D2D1_RECT_F const DIPs
        {
            get_dips_from_pixels(rect.left),
            get_dips_from_pixels(rect.top),
            get_dips_from_pixels(rect.right),
            get_dips_from_pixels(rect.bottom)
        };

        d2d_factory.get_render_target().DrawRectangle(DIPs, &d2d_factory.get_brush(c), static_cast<float>(stroke_width));
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

    int GraphicsDirect2D::get_screen_width() const noexcept
    {
        RECT const area_size{ d2d_factory.get_render_area_size() };
        return area_size.right - area_size.left;
    }

    int GraphicsDirect2D::get_screen_height() const noexcept
    {
        RECT const area_size{ d2d_factory.get_render_area_size() };
        return area_size.bottom - area_size.top;
    }
}