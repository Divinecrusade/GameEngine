#include "Blow.hpp"


Blow::Blow(Vec2i const& pos, GameEngine::Animation const& anim, GameEngine::Colour chroma)
:
pos{ pos },
blow_effect{ std::make_pair(anim, chroma) }
{ }

void Blow::draw(GameEngine::Interfaces::IGraphics2D& gfx, Rec2i const& clip) const
{
    GameEngine::Surface const& frame{ blow_effect.first.get_cur_frame() };
    gfx.draw_sprite_excluding_color(Vec2i{ pos.x - static_cast<int>(frame.get_width() / 2U), pos.y - static_cast<int>(frame.get_height() / 2U) }, frame, blow_effect.second, clip);
}

void Blow::update(float dt)
{
    blow_effect.first.update(dt);
}

bool Blow::is_ended() const noexcept
{
    return blow_effect.first.is_finished();
}

void Blow::throw_ball(Ball& ball) const
{
    auto const new_dir{ static_cast<Vec2f>(pos) - ball.get_direction() };
    ball.change_direction(new_dir);
    ball.accelerate(BALL_ACCELERATION);
}

Blow::Rec2i Blow::get_collision_box() const
{
    return Rec2i::get_from_center(pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT);
}
