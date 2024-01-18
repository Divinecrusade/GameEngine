#pragma once

#include "IGraphics2D.hpp"
#include "Direct2DFactory.hpp"


class GraphicsDirect2D : public IGraphics2D
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

    virtual void set_pixel(unsigned x, unsigned y, Color c) override;
    virtual Color get_pixel(unsigned x, unsigned y) const override;

    virtual void draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w, Color c) override;

    virtual void fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c) override;
    virtual void draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c) override;

private:
    
    bool composing_frame{ false };

    Direct2DFactory D2DFactory;
    Direct2DResources D2DResources;
};