#pragma once

#include "KeyColor.hpp"
#include "Vector2D.hpp"
#include "Rectangle2D.hpp"


namespace GameEngine2D
{
    namespace Interfaces
    {
        __interface IGraphics2D
        {
        public:

            virtual int get_screen_width() const = 0;
            virtual int get_screen_height() const = 0;

            virtual void draw_line(Geometry::Vector2D<int> const& beg, Geometry::Vector2D<int> const& end, int stroke_width, Colour c) = 0;

            virtual void fill_rectangle(Geometry::Rectangle2D<int> const& rect, Colour c) = 0;
            // TODO: add stroke width for rectangle drawing
            virtual void draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_witdth, Colour c) = 0;
        };
    }
}