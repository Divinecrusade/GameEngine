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
    }

    void GraphicsDirect2D::end_frame() noexcept
    {
        assert(composing_frame);

        composing_frame = false;
        d2d_factory.get_render_target().EndDraw();
    }

    int GraphicsDirect2D::get_screen_width() const noexcept
    {
        RECT const area_size{ d2d_factory.get_render_area_size() };
        assert(area_size.right > area_size.left);
        return area_size.right - area_size.left;
    }

    int GraphicsDirect2D::get_screen_height() const noexcept
    {
        RECT const area_size{ d2d_factory.get_render_area_size() };
        assert(area_size.bottom > area_size.top);
        return area_size.bottom - area_size.top;
    }

    void GraphicsDirect2D::draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(beg.x >= 0 && beg.x <= get_screen_width());
        assert(beg.y >= 0 && beg.y <= get_screen_height());
        assert(end.x >= 0 && end.x <= get_screen_width());
        assert(end.y >= 0 && end.y <= get_screen_height());
        assert(stroke_width > 0);

        d2d_factory.get_render_target().DrawLine(convert(beg), convert(end), &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GameEngine::GraphicsDirect2D::draw_line(Geometry::Vector2D<int> beg, Geometry::Vector2D<int> end, int stroke_width, Colour c, Geometry::Rectangle2D<int> const& clipping_area)
    {
        // Yeah, I know that algorithm below for clipping is dumb
        // But I'm in hurry, so let it be

        assert(composing_frame);
        assert(beg.x >= 0 && beg.x <= get_screen_width());
        assert(beg.y >= 0 && beg.y <= get_screen_height());
        assert(end.x >= 0 && end.x <= get_screen_width());
        assert(end.y >= 0 && end.y <= get_screen_height());
        assert(stroke_width > 0);

        int const delta_y{ max(beg.y, end.y) - min(beg.y, beg.y) };
        int const delta_x{ max(beg.x, end.x) - min(beg.x, end.x) };


        if (delta_x == 0)
        {
            if (beg.y > end.y) std::swap(beg, end);

            if (beg.x < min(clipping_area.left, clipping_area.right) || beg.x > max(clipping_area.left, clipping_area.right)) return;
            if (beg.y > max(clipping_area.bottom, clipping_area.top) || end.y < min(clipping_area.bottom, clipping_area.top)) return;

            beg.y = std::clamp(beg.y, min(clipping_area.bottom, clipping_area.top), max(clipping_area.bottom, clipping_area.top));
            end.y = std::clamp(end.y, min(clipping_area.bottom, clipping_area.top), max(clipping_area.bottom, clipping_area.top));
        }
        else if (delta_y == 0)
        {
            if (beg.x > end.x) std::swap(beg, end);

            if (beg.y < min(clipping_area.bottom, clipping_area.top) || beg.y > max(clipping_area.bottom, clipping_area.top)) return;
            if (beg.x > max(clipping_area.left, clipping_area.right) || beg.x < min(clipping_area.left, clipping_area.right)) return;

            beg.x = std::clamp(beg.x, min(clipping_area.left, clipping_area.right), max(clipping_area.left, clipping_area.right));
            end.x = std::clamp(beg.x, min(clipping_area.left, clipping_area.right), max(clipping_area.left, clipping_area.right));
        }
        else
        {
            if (!clipping_area.contains(beg) || !clipping_area.contains(end)) return;
        }

        draw_line(beg, end, stroke_width, c);
    }

    void GraphicsDirect2D::fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0 && rect.left <= get_screen_width());
        assert(rect.right >= 0 && rect.right <= get_screen_width());
        assert(rect.bottom >= 0 && rect.bottom <= get_screen_height());
        assert(rect.top >= 0 && rect.top <= get_screen_height());
        assert(rect.left <= rect.right);
        assert(rect.top <= rect.bottom);

        d2d_factory.get_render_target().FillRectangle(convert(rect), &d2d_factory.get_brush(c));
    }

    void GraphicsDirect2D::draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_width, Colour c)
    {
        assert(composing_frame);
        assert(rect.left >= 0 && rect.left <= get_screen_width());
        assert(rect.right >= 0 && rect.right <= get_screen_width());
        assert(rect.bottom >= 0 && rect.bottom <= get_screen_height());
        assert(rect.top >= 0 && rect.top <= get_screen_height());
        assert(rect.left <= rect.right);
        assert(rect.top <= rect.bottom);

        d2d_factory.get_render_target().DrawRectangle(convert(rect), &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c)
    {
        assert(composing_frame);
        assert(center.x >= 0 && center.x <= get_screen_width());
        assert(center.y >= 0 && center.y <= get_screen_height());
        assert(radius_x > 0);
        assert(radius_y > 0);   
        assert(center.x + radius_x <= get_screen_width());
        assert(center.x - radius_x >= 0);
        assert(center.y + radius_y <= get_screen_height());
        assert(center.y - radius_y >= 0);

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
        assert(center.x >= 0 && center.x <= get_screen_width());
        assert(center.y >= 0 && center.y <= get_screen_height());
        assert(radius_x > 0);
        assert(radius_y > 0);
        assert(center.x + radius_x <= get_screen_width());
        assert(center.x - radius_x >= 0);
        assert(center.y + radius_y <= get_screen_height());
        assert(center.y - radius_y >= 0);

        D2D1_ELLIPSE const DIPs
        {
            convert(center),
            get_dips_from_pixels(radius_x),
            get_dips_from_pixels(radius_y)
        };
        d2d_factory.get_render_target().DrawEllipse(DIPs, &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::draw_sprite(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Geometry::Rectangle2D<int> const& clipping_area)
    {
        assert(composing_frame);
        assert(left_top_pos.x >= 0 && left_top_pos.x <= get_screen_width());
        assert(left_top_pos.y >= 0 && left_top_pos.y <= get_screen_height());
        assert(clipping_area.left >= 0 && clipping_area.left <= get_screen_width());
        assert(clipping_area.right >= 0 && clipping_area.right <= get_screen_width());
        assert(clipping_area.bottom >= 0 && clipping_area.bottom <= get_screen_height());
        assert(clipping_area.top >= 0 && clipping_area.top <= get_screen_height());
        assert(clipping_area.left <= clipping_area.right);
        assert(clipping_area.top <= clipping_area.bottom);

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
                                                   D2D1::RectF
                                                   (
                                                       get_dips_from_pixels((part_to_draw.left   - left_top_pos.x) > 0 ? (part_to_draw.left   - left_top_pos.x) : 0),
                                                       get_dips_from_pixels((part_to_draw.top    - left_top_pos.y) > 0 ? (part_to_draw.top    - left_top_pos.y) : 0),
                                                       get_dips_from_pixels((part_to_draw.right  - left_top_pos.x) > 0 ? (part_to_draw.right  - left_top_pos.x) : 0),
                                                       get_dips_from_pixels((part_to_draw.bottom - left_top_pos.y) > 0 ? (part_to_draw.bottom - left_top_pos.y) : 0)
                                                   ));
    }

    void GraphicsDirect2D::draw_sprite_excluding_color(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Colour chroma, Geometry::Rectangle2D<int> const& clipping_area)
    {
        assert(composing_frame);
        assert(left_top_pos.x >= 0 && left_top_pos.x <= get_screen_width());
        assert(left_top_pos.y >= 0 && left_top_pos.y <= get_screen_height());
        assert(clipping_area.left >= 0 && clipping_area.left <= get_screen_width());
        assert(clipping_area.right >= 0 && clipping_area.right <= get_screen_width());
        assert(clipping_area.bottom >= 0 && clipping_area.bottom <= get_screen_height());
        assert(clipping_area.top >= 0 && clipping_area.top <= get_screen_height());
        assert(clipping_area.left <= clipping_area.right);
        assert(clipping_area.top <= clipping_area.bottom);

        ID2D1Bitmap& bmp_img { d2d_factory.get_bitmap(sprite) };
        ID2D1Bitmap& bmp_mask{ d2d_factory.get_bitmap(make_mask(sprite, chroma)) };
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
                                                        D2D1::RectF
                                                        (
                                                            get_dips_from_pixels((part_to_draw.left   - left_top_pos.x) > 0 ? (part_to_draw.left   - left_top_pos.x) : 0),
                                                            get_dips_from_pixels((part_to_draw.top    - left_top_pos.y) > 0 ? (part_to_draw.top    - left_top_pos.y) : 0),
                                                            get_dips_from_pixels((part_to_draw.right  - left_top_pos.x) > 0 ? (part_to_draw.right  - left_top_pos.x) : 0),
                                                            get_dips_from_pixels((part_to_draw.bottom - left_top_pos.y) > 0 ? (part_to_draw.bottom - left_top_pos.y) : 0)
                                                        ));
        d2d_factory.get_render_target().SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }

    SurfaceView GameEngine::GraphicsDirect2D::make_mask(SurfaceView sprite, Colour c_req)
    {
        KeyColor const* const raw_ptr_to_srf{ std::to_address(sprite.begin()) };
        auto it{ masks.find(raw_ptr_to_srf) };
        if (it == masks.end())
        {
            for (auto& c_cur : (it = masks.emplace(raw_ptr_to_srf, sprite).first).operator*().second)
            {
                if (Colour::is_equal_except_one_component(c_cur, c_req))
                {
                    c_cur[Colour::ComponentIndex::A] = Colour::MIN_COLOUR_DEPTH;
                }
            }
        }

        return it->second;
    }

    void GraphicsDirect2D::draw_polygon(std::vector<Geometry::Vector2D<int>> const& points, int stroke_width, Colour c)
    {
        assert(!points.empty());
        assert(std::ranges::find_if(points, [width = get_screen_width(), height = get_screen_height()](auto const& val) { return val.x >= 0 && val.x <= width && val.y >= 0 && val.y <= height; }) == points.end());
        assert(stroke_width > 0);

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
        assert(!points.empty());
        assert(std::ranges::find_if(points, [width = get_screen_width(), height = get_screen_height()](auto const& val) { return !(val.x >= 0 && val.x <= width && val.y >= 0 && val.y <= height); }) == points.end());

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

    void GraphicsDirect2D::draw_text(std::wstring_view text, Colour c, DWriteFontNames font, int font_size, int font_weight, Geometry::Rectangle2D<int> const& clipping_area, DWriteFontStyles style, DWriteFontStretch stretch, DWriteTextHorizontalAlignment align1, DWriteTextVerticalAlignment align2)
    {
        assert(text.size() > 0);
        assert(font_size >= 0);
        assert(font_weight >= 1 && font_weight <= 999);
        assert(clipping_area.left >= 0 && clipping_area.left <= get_screen_width());
        assert(clipping_area.right >= 0 && clipping_area.right <= get_screen_width());
        assert(clipping_area.bottom >= 0 && clipping_area.bottom <= get_screen_height());
        assert(clipping_area.top >= 0 && clipping_area.top <= get_screen_height());
        assert(clipping_area.left <= clipping_area.right);
        assert(clipping_area.top <= clipping_area.bottom);

        IDWriteTextFormat& text_format{ d2d_factory.get_text_format(font, get_dips_from_pixels(font_size), font_weight, style, stretch) };
        text_format.SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(align1));
        text_format.SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(align2));
        d2d_factory.get_render_target().DrawTextW
        (
            text.data(),
            static_cast<UINT32>(text.size()),
            &text_format,
            D2D1::RectF
            (
                get_dips_from_pixels(clipping_area.left),
                get_dips_from_pixels(clipping_area.top),
                get_dips_from_pixels(clipping_area.right),
                get_dips_from_pixels(clipping_area.bottom)
            ),
            &d2d_factory.get_brush(c)
        );
    }
}