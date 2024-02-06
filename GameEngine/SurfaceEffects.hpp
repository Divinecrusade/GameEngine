#pragma once

#include "KeyColor.hpp"
#include <functional>


namespace GameEngine
{
    namespace SurfaceEffects
    {
        class PixelManipulation
        {
        public:

            PixelManipulation() = delete;
            PixelManipulation(std::function<Colour(Colour)> const& functor) noexcept
            :
            functor{ functor }
            { }
            PixelManipulation(PixelManipulation const&) noexcept = default;
            PixelManipulation(PixelManipulation&&) noexcept = default;

            PixelManipulation& operator=(PixelManipulation const&) noexcept = default;
            PixelManipulation& operator=(PixelManipulation&&) noexcept = default;

            virtual ~PixelManipulation() noexcept = default;

            inline virtual Colour operator()(Colour pixel) const noexcept
            {
                return functor(pixel);
            }

        private:
            
            std::function<Colour(Colour)> functor;
        };
    
        class Filter : public PixelManipulation
        {
        public:

            Filter() = delete;
            Filter(Filter const&) noexcept = default;
            Filter(Filter&&) noexcept = default;

            Filter& operator=(Filter const&) = delete;
            Filter& operator=(Filter&&) = delete;
            Filter(Colour chroma = Colours::MAGENTA)
            :
            PixelManipulation
            { 
                [chroma](Colour pixel) -> Colour
                {
                    if (pixel == chroma) pixel[Colour::ComponentIndex::A] = Colour::MIN_COLOUR_DEPTH;

                    return pixel;
                }
            }
            { }

            virtual ~Filter() noexcept = default;
        };
    }
}