#pragma once

#include "IGraphics2D.hpp"
#include "Rectangle2D.hpp"


namespace GameEngine::Interfaces
{
    class IDrawable
    {
    public:

        virtual void draw(IGraphics2D& gfx, [[ maybe_unused ]] std::optional<Geometry::Rectangle2D<int>> const& clipping_area = std::nullopt) const = 0;
    };
}