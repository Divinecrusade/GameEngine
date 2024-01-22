#pragma once

#include "IGraphics2D.hpp"
#include "Direct2DFactory.hpp"


namespace GameEngine2D
{
    class GraphicsDirect2D : public Interfaces::IGraphics2D
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

        virtual unsigned get_screen_width() const noexcept override;
        virtual unsigned get_screen_height() const noexcept override;

        virtual void draw_line(Geometry::Vector2D<float> const& beg, Geometry::Vector2D<float> const& end, unsigned stroke_width, Colour c) override;

        virtual void fill_rectangle(Geometry::Rectangle2D<float> const& rect, Colour c) override;
        virtual void draw_rectangle(Geometry::Rectangle2D<float> const& rect, unsigned stroke_witdth, Colour c) override;

    private:
    
        bool composing_frame{ false };

        Direct2DFactory d2d_factory;
    };
}