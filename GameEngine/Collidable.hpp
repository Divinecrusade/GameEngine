#pragma once

#include "Rectangle2D.hpp"


namespace GameEngine::Abstract
{
    struct Collidable
    {
    public:

        virtual Geometry::Rectangle2D<int> get_collision_box() const = 0;

        bool is_collided_with(Collidable const& obj) const noexcept
        {
            return this->get_collision_box().is_colided_with(obj.get_collision_box());
        }
        int get_sqr_distance(Collidable const& obj)  const noexcept
        {
            return (this->get_collision_box().get_center() - obj.get_collision_box().get_center()).get_square_length();
        }
    };
}