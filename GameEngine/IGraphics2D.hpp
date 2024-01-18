#pragma once

#include <d2d1.h>


using Color = D2D1::ColorF;

__interface IGraphics2D
{
public:

    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    virtual unsigned get_screen_width() const = 0;
    virtual unsigned get_screen_height() const = 0;

    virtual void set_pixel(unsigned x, unsigned y, Color c) = 0;
    virtual Color get_pixel(unsigned x, unsigned y) const = 0;

    virtual void draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w, Color c) = 0;

    virtual void fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c) = 0;
    virtual void draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Color c) = 0;
};