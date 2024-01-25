#pragma once

#include "KeyColor.hpp"
#include "Vector2D.hpp"
#include "Rectangle2D.hpp"


namespace GameEngine
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
            virtual void draw_rectangle(Geometry::Rectangle2D<int> const& rect, int stroke_witdth, Colour c) = 0;

            virtual void fill_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, Colour c) = 0;
            virtual void draw_ellipse(Geometry::Vector2D<int> const& center, int radius_x, int radius_y, int stroke_width, Colour c) = 0;
        };
    }
}