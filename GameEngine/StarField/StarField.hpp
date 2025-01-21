#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>


class StarField : public GameEngine::Game
{
public:

    static constexpr std::wstring_view GAME_TITLE{ L"StarField" };

public:

    StarField(HINSTANCE hInstance, int nCmdShow);

private:

    static GameEngine::Interfaces::IWindow& get_window(HINSTANCE hInstance = NULL, int nCmdShow = 0);
    static GameEngine::Interfaces::IFramableGraphics2D& get_graphics();

private:

};
