#include "Brick.hpp"
#include <algorithm>
#include <cmath>


Brick::Brick() noexcept
:
collision_box{ DEFAULT_POS, WIDTH, HEIGHT },
c{ DEFAULT_COLOUR }
{ }

Brick::Brick(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Colour c) noexcept
:
collision_box{ pos, WIDTH, HEIGHT },
c{ c }
{ }

Brick::Brick(Brick const& b) noexcept
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

Brick& Brick::operator=(Brick const& b) noexcept
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

bool Brick::is_colided_with(Ball const& ball) const noexcept
{
    return ball.get_collision_box().is_colided_with(collision_box);
}

void Brick::handle_collision(Ball& ball) const noexcept
{
    assert(is_colided_with(ball));

    auto const ball_position{ ball.get_center() };
    if ((ball_position.x >= collision_box.left && ball_position.x <= collision_box.right)
        ||
        (std::signbit(ball.get_velocity().x) == GameEngine::Geometry::Auxiliry::signbit((ball.get_center() - collision_box.get_center()).x)))
    {
        ball.inverse_y();
    }
    else
    {
        ball.inverse_x();
    }
}

int Brick::get_sqr_distance(Ball const& ball) const noexcept
{
    return (ball.get_center() - collision_box.get_center()).get_square_length();
}

void Brick::swap(Brick&& b) noexcept
{
    std::swap(collision_box, b.collision_box);
    std::swap(c, b.c);
}

bool operator==(Brick const& lhs, Brick const& rhs) noexcept
{
    return lhs.collision_box == rhs.collision_box && lhs.c == rhs.c;
}

bool operator!=(Brick const& lhs, Brick const& rhs) noexcept
{
    return !(lhs == rhs);
}
