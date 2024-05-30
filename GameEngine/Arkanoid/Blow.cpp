#include "Blow.hpp"


Blow::Blow(Vec2i const& pos, std::shared_ptr<std::vector<GameEngine::Surface> const> const& frames, GameEngine::Colour chroma)
:
pos{ pos },
blow_effect{ std::make_pair(GameEngine::Animation{ frames, DURATION / static_cast<float>(frames->size()) }, chroma) }
{ }

void Blow::draw(GameEngine::Interfaces::IGraphics2D& gfx, Rec2i const& clip) const
{
    auto frame{ blow_effect.first.get_cur_frame() };
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
    auto new_dir{ ball.get_collision_box().get_center() + pos };
    ball.change_direction(new_dir);
    ball.accelerate(BALL_ACCELERATION);
}