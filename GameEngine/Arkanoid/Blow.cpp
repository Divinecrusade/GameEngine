#include "Blow.hpp"


Blow::Blow(Vec2i const& pos, GameEngine::Animation const& anim, GameEngine::Colour chroma) noexcept
:
pos{ pos },
blow_effect{ anim, chroma }
{ }

void Blow::draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<Rec2i> const& clipping_area) const
{
    assert(clipping_area.has_value());

    GameEngine::SurfaceView const frame{ blow_effect.first.get_cur_frame() };
    gfx.draw_sprite_excluding_color(Vec2i{ pos.x - static_cast<int>(frame.get_width() / 2U), pos.y - static_cast<int>(frame.get_height() / 2U) }, frame, blow_effect.second, *clipping_area);
}

void Blow::update(float dt) noexcept
{
    blow_effect.first.update(dt);
}

bool Blow::is_ended() const noexcept
{
    return blow_effect.first.is_finished();
}

void Blow::throw_ball(Ball& ball) const noexcept
{
    assert(is_collided_with(ball));
    ball.change_direction({ static_cast<Vec2f>(pos) - ball.get_direction() });
    ball.accelerate(BALL_ACCELERATION);
}

Blow::Rec2i Blow::get_collision_box() const noexcept
{
    return Rec2i::get_from_center(pos, COLLISION_HALF_WIDTH, COLLISION_HALF_HEIGHT);
}
