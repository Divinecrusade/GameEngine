#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine2D::Interfaces::IWindow& window, GameEngine2D::Interfaces::IGraphics2D& graphics)
:
Game{ window, graphics },
field{ GameEngine2D::Geometry::Rectangle2D{ 0 + PADDING_LEFT, WINDOW_WIDTH - PADDING_RIGHT, WINDOW_HEIGHT - PADDING_BOTTOM, 0 + PADDING_TOP } }
{ }

void Arkanoid::render()
{
    Game::render();
    field.draw(gfx);
}
