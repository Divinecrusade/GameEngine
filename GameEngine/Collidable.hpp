#pragma once

#include "Rectangle2D.hpp"


namespace GameEngine::Abstract
{
    struct Collidable
    {
    public:

        Collidable() = default;
        Collidable(Collidable const&) = default;
        Collidable(Collidable&&) = default;

        Collidable& operator=(Collidable const&) = default;
        Collidable& operator=(Collidable&&) = default;

        virtual ~Collidable() = default;

        virtual Geometry::Rectangle2D<int> get_collision_box() const = 0;

        [[maybe_unused]] bool is_collided_with(Collidable const& obj) const noexcept
        {
            return this->get_collision_box().is_colided_with(obj.get_collision_box());
        }
        [[maybe_unused]] int get_sqr_distance(Collidable const& obj)  const noexcept
        {
            return (this->get_collision_box().get_center() - obj.get_collision_box().get_center()).get_square_length();
        }
    };
}