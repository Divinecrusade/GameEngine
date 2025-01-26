#pragma once

#include "Shape.hpp"


namespace GameEngine::Interfaces
{
    class IModel
    {
    public:

        virtual Shape get_shape() const = 0;
    };
}
