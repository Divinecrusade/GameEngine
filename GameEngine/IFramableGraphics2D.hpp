#pragma once

#include "IGraphics2D.hpp"


namespace GameEngine::Interfaces
{
    __interface IFramableGraphics2D : public IGraphics2D
    {
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;
    };
}