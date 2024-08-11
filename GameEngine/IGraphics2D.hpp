#pragma once

#include "KeyColor.hpp"
#include "Vector2D.hpp"
#include "Rectangle2D.hpp"
#include "Surface.hpp"
#include "SurfaceView.hpp"
#include "DWriteFontNames.hpp"
#include "DWriteFontStyles.hpp"
#include "DWriteFontStretch.hpp"
#include "DWriteTextHorizontalAlignment.hpp"
#include "DWriteTextVerticalAlignment.hpp"

#include <vector>


namespace GameEngine::Interfaces
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
        
        virtual void draw_sprite(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Geometry::Rectangle2D<int> const& clipping_area) = 0;
        virtual void draw_sprite_excluding_color(Geometry::Vector2D<int> const& left_top_pos, SurfaceView sprite, Colour chroma, Geometry::Rectangle2D<int> const& clipping_area) = 0;
       
        virtual void draw_polygon(std::vector<Geometry::Vector2D<int>> const& points, int stroke_width, Colour c) = 0;
        virtual void fill_polygon(std::vector<Geometry::Vector2D<int>> const& points, Colour c) = 0;

        virtual void draw_text(std::wstring_view text, Colour c, DWriteFontNames font, int font_size, int font_weight, Geometry::Rectangle2D<int> const& clipping_area, DWriteFontStyles style = DWriteFontStyles::NORMAL, DWriteFontStretch stretch = DWriteFontStretch::NORMAL, DWriteTextHorizontalAlignment align1 = DWriteTextHorizontalAlignment::LEFT, DWriteTextVerticalAlignment align2 = DWriteTextVerticalAlignment::TOP) = 0;
    };
}