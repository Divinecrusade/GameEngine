#pragma once

#include "Vector2D.hpp"
#include "KeyColor.hpp"
#include "IGraphics2D.hpp"

#include <vector>


namespace GameEngine
{
    namespace Interfaces
    {
        __interface ISurface
        {
            void draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma = Colours::MAGENTA) const;
            std::vector<std::reference_wrapper<Colour>> get_bitmap() const;

            int get_width() const;
            int get_height() const;
        };
    }
}