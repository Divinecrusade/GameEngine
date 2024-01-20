#pragma once

#include <d2d1.h>


__interface IGraphics2D
{
public:

    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    virtual unsigned get_screen_width() const = 0;
    virtual unsigned get_screen_height() const = 0;

    virtual void draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w,
                           unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255U) = 0;

    virtual void fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, 
                                unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255U) = 0;
    virtual void draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, 
                                unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255U) = 0;
};