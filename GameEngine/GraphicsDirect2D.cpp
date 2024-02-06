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

        d2d_factory.get_render_target().DrawLine(p1, p2, &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
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

        d2d_factory.get_render_target().DrawRectangle(DIPs, &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c)
    {
        assert(composing_frame);
        assert(radius_x > 0);
        assert(radius_y > 0);   

        D2D1_ELLIPSE const DIPs
        {
            D2D1_POINT_2F
            {
                get_dips_from_pixels(center.x),
                get_dips_from_pixels(center.y)
            },
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
            D2D1_POINT_2F
            {
                get_dips_from_pixels(center.x),
                get_dips_from_pixels(center.y)
            },
            get_dips_from_pixels(radius_x),
            get_dips_from_pixels(radius_y)
        };
        d2d_factory.get_render_target().DrawEllipse(DIPs, &d2d_factory.get_brush(c), get_dips_from_pixels(stroke_width));
    }

    void GraphicsDirect2D::draw_sprite(Geometry::Vector2D<int> const& left_top_pos, Interfaces::ISurface const& sprite, Colour chroma)
    {
        assert(composing_frame);
        assert(left_top_pos.x >= 0);
        assert(left_top_pos.y >= 0);

        ID2D1Bitmap* bmp_picture{ nullptr };
        ID2D1Bitmap* bmp_mask{ nullptr };
        D2D1_SIZE_U const sizes{ D2D1::SizeU(sprite.get_width(), sprite.get_height()) };
        UINT32 const pitch { sizes.width * sizeof(UINT8) * 4 };
        FLOAT dpiX{ };
        FLOAT dpiY{ };
        d2d_factory.get_render_target().GetDpi(&dpiX, &dpiY);
        D2D1_BITMAP_PROPERTIES const props
        { 
            D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            dpiX, dpiY
        };
        auto mask{ sprite.get_pixels(GameEngine::SurfaceEffects::Filter{ Colours::WHITE })};
        auto img{ sprite.get_pixels()};
        d2d_factory.get_render_target().CreateBitmap(sizes, reinterpret_cast<void const*>(img.get()), pitch, props, &bmp_picture);
        d2d_factory.get_render_target().CreateBitmap(sizes, reinterpret_cast<void const*>(mask.data()), pitch, props, &bmp_mask);
        
        D2D1_BITMAP_BRUSH_PROPERTIES props_for_img
        {
            D2D1::BitmapBrushProperties
            (
                D2D1_EXTEND_MODE_CLAMP,
                D2D1_EXTEND_MODE_CLAMP,
                D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
            )
        };
        ID2D1BitmapBrush* brush{ nullptr };
        d2d_factory.get_render_target().CreateBitmapBrush(bmp_picture, props_for_img, &brush);

        D2D1_RECT_F rect{ D2D1::RectF(get_dips_from_pixels(left_top_pos.x), get_dips_from_pixels(left_top_pos.y), get_dips_from_pixels(left_top_pos.x + sprite.get_width()), get_dips_from_pixels(left_top_pos.y + sprite.get_height())) };
        
        d2d_factory.get_render_target().SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        assert(bmp_mask && bmp_picture);
        d2d_factory.get_render_target().FillOpacityMask(bmp_mask, brush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS, &rect, &rect);
        d2d_factory.get_render_target().SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        //assert(bitmap);
        //d2d_factory.get_render_target().DrawBitmap(bitmap, D2D1::RectF(get_dips_from_pixels(left_top_pos.x), get_dips_from_pixels(left_top_pos.y), 
        //                                                               get_dips_from_pixels(left_top_pos.x + sprite.get_width()), get_dips_from_pixels(left_top_pos.y + sprite.get_height())));
    }
}