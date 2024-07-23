#pragma once

#include "IGraphics2D.hpp"
#include "Rectangle2D.hpp"
#include <optional>


namespace GameEngine::Interfaces
{
    __interface IDrawable
    {
    public:

        virtual void draw(IGraphics2D& gfx, [[ maybe_unused ]] std::optional<Geometry::Rectangle2D<int>> const& clipping_area = std::nullopt) const = 0;
    };
}