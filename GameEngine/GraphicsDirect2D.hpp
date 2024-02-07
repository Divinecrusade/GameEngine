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

        virtual ~GraphicsDirect2D() = default;

        virtual void begin_frame() override;
        virtual void end_frame() override;

        virtual int get_screen_width() const noexcept override;
        virtual int get_screen_height() const noexcept override;

        virtual void draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c) override;

        virtual void fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c) override;
        virtual void draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_witdth, Colour c) override;

        virtual void fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c) override;
        virtual void draw_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, int stroke_width, Colour c) override;

        virtual void draw_sprite(Geometry::Vector2D<int> const& left_top_pos, Interfaces::ISurface const& sprite) override;
        virtual void draw_sprite_excluding_color(Geometry::Vector2D<int> const& left_top_pos, Interfaces::ISurface const& sprite, Colour chroma) override;

    private:

        std::shared_ptr<Colour const[]> make_mask(Interfaces::ISurface const& sprite, Colour c_req);

        float get_dips_from_pixels(int px) const
        {
            return static_cast<float>(px) / (static_cast<float>(GetDpiForWindow(d2d_factory.get_render_target().GetHwnd())) / USER_DEFAULT_SCREEN_DPI);
        }

    private:
    
        bool composing_frame{ false };

        mutable Direct2DFactory d2d_factory;
    };
}