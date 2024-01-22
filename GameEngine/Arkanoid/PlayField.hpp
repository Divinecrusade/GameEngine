#pragma once

#include "../Rectangle2D.hpp"


class PlayField final
{
public:

    PlayField() = delete;
    PlayField(PlayField const&) = delete;
    PlayField(PlayField&&) = delete;

    PlayField& operator=(PlayField const&) = delete;
    PlayField& operator=(PlayField&&) = delete;

private:


    GameEngine2D::Geometry::Rectangle2D<unsigned> const collision_box;
};