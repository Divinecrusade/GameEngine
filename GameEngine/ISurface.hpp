#pragma once

#include "Vector2D.hpp"
#include "KeyColor.hpp"
#include "SurfaceEffects.hpp"

#include <memory>


namespace GameEngine
{
    namespace Interfaces
    {
        __interface ISurface
        {
        public:

            virtual std::shared_ptr<Colour const[] > get_pixels() const = 0;
            virtual std::vector<Colour> get_pixels(SurfaceEffects::PixelManipulation const& effect) const = 0;

            virtual size_t get_width() const = 0;
            virtual size_t get_height() const = 0;
        };
    }
}