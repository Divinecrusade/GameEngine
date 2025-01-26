#pragma once

#include <Vector2D.hpp>
#include <KeyColor.hpp>
#include <vector>
#include <cassert>
#include <numbers>
#include <cmath>


class Star
{
private:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;

public:

    Star(Vec2f const& center, float outer_radius, int flares_count, GameEngine::Colour init_border_colour);

private:

    std::vector<Vec2f> model{ };
    GameEngine::Colour border_colour{ };
};