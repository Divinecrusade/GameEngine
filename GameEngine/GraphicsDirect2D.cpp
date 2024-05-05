#include "GraphicsDirect2D.hpp"


namespace GameEngine
{
    GraphicsDirect2D::GraphicsDirect2D(HWND hWnd)
    :
    d2d_factory{ hWnd }
    { }

    void GraphicsDirect2D::begin_frame()
    {
        assert(!composing_frame);

        composing_frame = true;
        d2d_factory.get_render_target().BeginDraw();
        d2d_factory.get_render_target().Clear();
        d2d_factory.invalidate_resources();
    }

    void GraphicsDirect2D::end_frame()
    {
        assert(composing_frame);

        composing_frame = false;
        d2d_factory.get_render_target().EndDraw();
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

    void GraphicsDirect2D::draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(beg.x >= 0);
        assert(beg.y >= 0);
        assert(end.y >= 0);
        assert(end.x >= 0);
        assert(stroke_width > 0);

        d2d_factory.get_render_target().DrawLine(convert(beg), convert(end), &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0);
        assert(rect.right >= 0);
        assert(rect.bottom >= 0);
        assert(rect.top >= 0);

        d2d_factory.get_render_target().FillRectangle(convert(rect), &d2d_factory.get_brush(c));
    }

    void GraphicsDirect2D::draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0);
        assert(rect.right >= 0);
        assert(rect.bottom >= 0);
        assert(rect.top >= 0);

        d2d_factory.get_render_target().DrawRectangle(convert(rect), &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c)
    {
        assert(composing_frame);
        assert(radius_x > 0);
        assert(radius_y > 0);   

        D2D1_ELLIPSE const DIPs
        {
            convert(center),
            get_dips_from_pixels(radius_x),
            get_dips_from_pixels(radius_y)
        };
        d2d_factory.get_render_target().FillEllipse(DIPs, &d2d_factory.get_brush(c));
    }

    void GraphicsDirect2D::draw_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(radius_x > 0);
        assert(radius_y > 0);

        D2D1_ELLIPSE const DIPs
        {
            convert(center),
            get_dips_from_pixels(radius_x),
            get_dips_from_pixels(radius_y)
        };
        d2d_factory.get_render_target().DrawEllipse(DIPs, &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::draw_sprite(Geometry::Vector2D<int> const& left_top_pos, Interfaces::ISurface const& sprite, Geometry::Rectangle2D<int> const& clipping_area)
    {
        assert(composing_frame);

        ID2D1Bitmap& bmp_img{ d2d_factory.get_bitmap(sprite) };

        GameEngine::Geometry::Rectangle2D<int> const drawing_area
        {
                left_top_pos.x,
                left_top_pos.x + static_cast<int>(sprite.get_width()),
                left_top_pos.y + static_cast<int>(sprite.get_height()),
                left_top_pos.y
        };
        GameEngine::Geometry::Rectangle2D<int> const part_to_draw{ clip(drawing_area, clipping_area) };
        
        d2d_factory.get_render_target().DrawBitmap(&bmp_img, convert(part_to_draw), 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                                                   D2D1::RectF(get_dips_from_pixels(max(part_to_draw.left - left_top_pos.x, 0)), get_dips_from_pixels(max(part_to_draw.top - left_top_pos.y, 0)), get_dips_from_pixels(max(part_to_draw.right - left_top_pos.x, 0)), get_dips_from_pixels(max(part_to_draw.bottom - left_top_pos.y, 0))));
    }

    void GraphicsDirect2D::draw_sprite_excluding_color(Geometry::Vector2D<int> const& left_top_pos, Interfaces::ISurface const& sprite, Colour chroma, Geometry::Rectangle2D<int> const& clipping_area)
    {
        assert(composing_frame);

        ID2D1Bitmap& bmp_img { d2d_factory.get_bitmap(sprite) };
        ID2D1Bitmap& bmp_mask{ d2d_factory.get_bitmap(Surface{ sprite.get_width(), sprite.get_height(), make_mask(sprite, chroma) }) };
        ID2D1BitmapBrush& brush{ d2d_factory.get_bitmapbrush(bmp_img) };


        GameEngine::Geometry::Rectangle2D<int> const drawing_area
        {
                left_top_pos.x,
                left_top_pos.x + static_cast<int>(sprite.get_width()),
                left_top_pos.y + static_cast<int>(sprite.get_height()),
                left_top_pos.y
        };
        GameEngine::Geometry::Rectangle2D<int> const part_to_draw{ clip(drawing_area, clipping_area) };

        brush.SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(get_dips_from_pixels(left_top_pos.x), get_dips_from_pixels(left_top_pos.y))));
        d2d_factory.get_render_target().SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        d2d_factory.get_render_target().FillOpacityMask(&bmp_mask, &brush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS, convert(part_to_draw), 
                                                        D2D1::RectF(get_dips_from_pixels(max(part_to_draw.left - left_top_pos.x, 0)), get_dips_from_pixels(max(part_to_draw.top - left_top_pos.y, 0)), get_dips_from_pixels(max(part_to_draw.right - left_top_pos.x, 0)), get_dips_from_pixels(max(part_to_draw.bottom - left_top_pos.y, 0))));
        d2d_factory.get_render_target().SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }

    std::shared_ptr<Colour const []> GameEngine::GraphicsDirect2D::make_mask(Interfaces::ISurface const& sprite, Colour c_req)
    {
        size_t const width{ sprite.get_width() };
        size_t const height{ sprite.get_height() };
        size_t const n_pixels{ width * height };

        auto const img{ sprite.get_pixels() };

        std::shared_ptr<Colour[]> mask{ new Colour[n_pixels] };
        
        for (size_t y{ 0U }; y != height; ++y)
        for (size_t x{ 0U }; x != width; ++x)
        {
            Colour c_cur{ img[static_cast<ptrdiff_t>(y * width + x)] };
            if (Colour::is_equal_except_one_component(c_cur, c_req))
            {
                c_cur[Colour::ComponentIndex::A] = Colour::MIN_COLOUR_DEPTH; 
            }
            
            mask[static_cast<ptrdiff_t>(y * width + x)] = (c_cur);
        }

        return mask;
    }

    Geometry::Rectangle2D<int> GraphicsDirect2D::clip(Geometry::Rectangle2D<int> const& drawing_area, Geometry::Rectangle2D<int> const& clipping_area)
    {
        return Geometry::Rectangle2D<int>{ max(drawing_area.left, clipping_area.left), min(drawing_area.right, clipping_area.right), min(drawing_area.bottom, clipping_area.bottom), max(drawing_area.top, clipping_area.top) };
    }

    void GraphicsDirect2D::draw_polygon(std::vector<Geometry::Vector2D<int>> const& points, int stroke_width, Colour c)
    {
        d2d_factory.open_sink();
        ID2D1GeometrySink& sink{ d2d_factory.get_sink() };
        
        auto it{ points.cbegin() };
        sink.BeginFigure(D2D1::Point2F(get_dips_from_pixels((*it).x), get_dips_from_pixels((*it).y)), D2D1_FIGURE_BEGIN_FILLED);

        for (; it != points.cend(); ++it)
        {
            sink.AddLine(D2D1::Point2F(get_dips_from_pixels((*it).x), get_dips_from_pixels((*it).y)));
        }
        sink.EndFigure(D2D1_FIGURE_END_CLOSED);
        d2d_factory.close_sink();

        ID2D1SolidColorBrush& brush{ d2d_factory.get_brush(c) };
        d2d_factory.get_render_target().DrawGeometry(&d2d_factory.get_geometry(), &brush, get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::fill_polygon(std::vector<Geometry::Vector2D<int>> const& points, Colour c)
    {
        d2d_factory.open_sink();
        ID2D1GeometrySink& sink{ d2d_factory.get_sink() };

        auto it{ points.cbegin() };
        sink.BeginFigure(D2D1::Point2F(get_dips_from_pixels((*it).x), get_dips_from_pixels((*it).y)), D2D1_FIGURE_BEGIN_FILLED);

        for (; it != points.cend(); ++it)
        {
            sink.AddLine(D2D1::Point2F(get_dips_from_pixels((*it).x), get_dips_from_pixels((*it).y)));
        }
        sink.EndFigure(D2D1_FIGURE_END_CLOSED);
        d2d_factory.close_sink();

        ID2D1SolidColorBrush& brush{ d2d_factory.get_brush(c) };
        d2d_factory.get_render_target().FillGeometry(&d2d_factory.get_geometry(), &brush);
    }
}