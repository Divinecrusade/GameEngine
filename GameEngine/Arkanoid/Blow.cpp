#include "Blow.hpp"

Blow::Blow(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Animation const& blow_effect, GameEngine::Colour chroma)
:
pos{ pos },
blow_effect{ blow_effect },
chroma{ chroma }
{ }

Blow::Blow(Blow const& other) noexcept
:
pos{ other.pos },
blow_effect{ other.blow_effect },
chroma{ other.chroma }
{ }

Blow::Blow(Blow && other_tmp) noexcept
:
pos{ other_tmp.pos },
blow_effect{ other_tmp.blow_effect },
chroma{ other_tmp.chroma }
{ }

void Blow::draw(GameEngine::Interfaces::IGraphics2D& gfx, GameEngine::Geometry::Rectangle2D<int> const& clip) const
{
    auto frame{ blow_effect.get_cur_frame() };
    gfx.draw_sprite_excluding_color(GameEngine::Geometry::Vector2D<int>{ pos.x - static_cast<int>(frame.get_width() / 2U), pos.y - static_cast<int>(frame.get_height() / 2U) }, frame, chroma, clip);
}

void Blow::update(float dt)
{
    blow_effect.update(dt);
}

bool Blow::is_ended() const noexcept
{
    return blow_effect.is_finished();
}

bool Blow::is_collided_with(Ball const& ball) const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT).is_colided_with(ball.get_collision_box());
}

bool Blow::is_collided_with(Paddle const& padd) const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT).is_colided_with(padd.get_collision_box());
}

bool Blow::is_collided_with(Missile const& missile) const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT).is_colided_with(missile.get_collision_box());
}

void Blow::throw_ball(Ball& ball) const
{
    auto new_dir{ ball.get_center() - pos };
    ball.change_direction(new_dir);
    ball.accelerate(BALL_ACCELERATION);
}