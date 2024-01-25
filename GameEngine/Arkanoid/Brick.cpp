#include "Brick.hpp"
#include <algorithm>

Brick::Brick()
:
collision_box{ GameEngine::Geometry::Vector2D<int>{ DEFAULT_POS_X, DEFAULT_POS_Y }, WIDTH, HEIGHT },
c{ DEFAULT_COLOUR }
{ }

Brick::Brick(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Colour c)
:
collision_box{ pos, WIDTH, HEIGHT },
c{ c }
{ }

Brick::Brick(Brick const& b)
:
collision_box{ b.collision_box },
c{ b.c }
{ }

Brick::Brick(Brick && b) noexcept
:
Brick::Brick()
{
    swap(std::move(b));
}

Brick& Brick::operator=(Brick const& b)
{
    if (this != &b)
    {
        collision_box = b.collision_box;
        c = b.c;
    }

    return *this;
}

Brick& Brick::operator=(Brick&& b) noexcept
{
    swap(std::move(b));
    return *this;
}

void Brick::draw(GameEngine::Interfaces::IGraphics2D & gfx) const
{
    gfx.fill_rectangle(collision_box.get_expanded(-VISUAL_PADDING), c);
}

void Brick::swap(Brick&& b) noexcept
{
    std::swap(collision_box, b.collision_box);
    std::swap(c, b.c);
}
