#pragma once

#include "KeyColor.hpp"


namespace GameEngine2D
{
    namespace Interfaces
    {
        __interface IGraphics2D
        {
        public:

            virtual void begin_frame() = 0;
            virtual void end_frame() = 0;

            virtual unsigned get_screen_width() const = 0;
            virtual unsigned get_screen_height() const = 0;

            virtual void draw_line(unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned w, Colour c) = 0;

            virtual void fill_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Colour c) = 0;
            // TODO: add stroke width for rectangle drawing
            virtual void draw_rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, Colour c) = 0;
        };
    }
}