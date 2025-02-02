#pragma once

#include "Shape.hpp"


namespace GameEngine
{
    class WorldTransformer final
    {
    public:

        WorldTransformer() = default;

        Shape transform(Shape model) const;

    private:

    };
}