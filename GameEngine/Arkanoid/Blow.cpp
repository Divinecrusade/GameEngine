#include "Blow.hpp"

Blow::Blow(GameEngine::Geometry::Vector2D<int> const& pos, GameEngine::Animation const& blow_effect, GameEngine::Colour chroma)
    :
    HALF_WIDTH_COLLISION{ static_cast<int>(blow_effect.get_cur_frame().get_width()) },
    HALF_HEIGHT_COLLISION{ static_cast<int>(blow_effect.get_cur_frame().get_height()) },
    pos{ pos },
    blow_effect{ blow_effect },
    chroma{ chroma }
{ }

Blow::Blow(Blow const& other) noexcept
:
HALF_WIDTH_COLLISION{ other.HALF_WIDTH_COLLISION },
HALF_HEIGHT_COLLISION{ other.HALF_HEIGHT_COLLISION },
pos{ other.pos },
blow_effect{ other.blow_effect },
chroma{ other.chroma }
{ }

Blow::Blow(Blow && other_tmp) noexcept
:
HALF_WIDTH_COLLISION{ other_tmp.HALF_WIDTH_COLLISION },
HALF_HEIGHT_COLLISION{ other_tmp.HALF_HEIGHT_COLLISION },
pos{ other_tmp.pos },
blow_effect{ other_tmp.blow_effect },
chroma{ other_tmp.chroma }
{ }

void Blow::draw(GameEngine::Interfaces::IGraphics2D& gfx) const
{
    auto frame{ blow_effect.get_cur_frame() };
    gfx.draw_sprite_excluding_color(GameEngine::Geometry::Vector2D<int>{ pos.x - static_cast<int>(frame.get_width() / 2U), pos.y - static_cast<int>(frame.get_height() / 2U) }, frame, chroma);
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
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(pos, HALF_WIDTH_COLLISION, HALF_HEIGHT_COLLISION).is_colided_with(ball.get_collision_box());
}

bool Blow::is_collided_with(Paddle const& padd) const
{
    return GameEngine::Geometry::Rectangle2D<int>::get_from_center(pos, HALF_WIDTH_COLLISION, HALF_HEIGHT_COLLISION).is_colided_with(padd.get_collision_box());
}

void Blow::throw_ball(Ball& ball) const
{
    auto new_dir{ ball.get_center() - pos };
    ball.change_direction(new_dir);
    ball.accelerate(BALL_ACCELERATION);
}