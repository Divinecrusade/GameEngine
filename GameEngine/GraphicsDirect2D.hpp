#pragma once

#include "IFramableGraphics2D.hpp"
#include "Direct2DFactory.hpp"

#include "Surface.hpp"


namespace GameEngine
{
    class GraphicsDirect2D : public Interfaces::IFramableGraphics2D
    {
    public:

        GraphicsDirect2D(HWND hWnd);
        GraphicsDirect2D(GraphicsDirect2D const&) = delete;
        GraphicsDirect2D(GraphicsDirect2D&&) = delete;

        GraphicsDirect2D& operator=(GraphicsDirect2D const&) = delete;
        GraphicsDirect2D& operator=(GraphicsDirect2D&&) = delete;

        virtual ~GraphicsDirect2D() noexcept = default;

        void begin_frame() override;
        void end_frame()   override;

        int get_screen_width() const override;
        int get_screen_height() const override;

        void draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c) override;
        void draw_line(Geometry::Vector2D<int> beg, Geometry::Vector2D<int> end, int stroke_width, Colour c, Geometry::Rectangle2D<int> const& clipping_area) override;

        void fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c) override;
        void draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_witdth, Colour c) override;

        void fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c) override;
        void draw_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, int stroke_width, Colour c) override;

        void draw_sprite(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Geometry::Rectangle2D<int> const& clipping_area) override;
        void draw_sprite_excluding_color(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Colour chroma, Geometry::Rectangle2D<int> const& clipping_area) override;

        void draw_polygon(std::vector<Geometry::Vector2D<int>> const& points, int stroke_width, Colour c) override;
        void fill_polygon(std::vector<Geometry::Vector2D<int>> const& points, Colour c) override;

        void draw_text(std::wstring_view text, Colour c, DWriteFontNames font, int font_size, int font_weight, Geometry::Rectangle2D<int> const& clipping_area, DWriteFontStyles style = DWriteFontStyles::NORMAL, DWriteFontStretch stretch = DWriteFontStretch::NORMAL, DWriteTextHorizontalAlignment align1 = DWriteTextHorizontalAlignment::LEFT, DWriteTextVerticalAlignment align2 = DWriteTextVerticalAlignment::TOP) override;

    private:

        SurfaceView make_mask(SurfaceView sprite, Colour c_req);

        float get_dips_from_pixels(int px) const
        {
            return static_cast<float>(px) / (static_cast<float>(GetDpiForWindow(d2d_factory.get_render_target().GetHwnd())) / USER_DEFAULT_SCREEN_DPI);
        }
        D2D1_RECT_F convert(Geometry::Rectangle2D<int> const& source) const
        {
            return D2D1::RectF(get_dips_from_pixels(source.left), get_dips_from_pixels(source.top), get_dips_from_pixels(source.right), get_dips_from_pixels(source.bottom));
        }
        D2D1_POINT_2F convert(Geometry::Vector2D<int> const& source) const
        {
            return D2D1::Point2F(get_dips_from_pixels(source.x), get_dips_from_pixels(source.y));
        }

        static constexpr Geometry::Rectangle2D<int> clip(Geometry::Rectangle2D<int> const& drawing_area, Geometry::Rectangle2D<int> const& clipping_area) noexcept
        {
            return Geometry::Rectangle2D<int>
            {
                ((drawing_area.left   > clipping_area.left)   ? drawing_area.left   : clipping_area.left),
                ((drawing_area.right  < clipping_area.right)  ? drawing_area.right  : clipping_area.right),
                ((drawing_area.bottom < clipping_area.bottom) ? drawing_area.bottom : clipping_area.bottom),
                ((drawing_area.top    > clipping_area.top)    ? drawing_area.top    : clipping_area.top)
            };
        }

    private:
    
        bool composing_frame{ false };
        std::unordered_map<KeyColor const*, Surface> masks{ };

        mutable Direct2DFactory d2d_factory;
    };
}