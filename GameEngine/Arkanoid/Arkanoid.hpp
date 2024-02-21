#pragma once

#include "../Game.hpp"
#include "../GraphicsDirect2D.hpp"
#include "../IWindow.hpp"
#include "../FrameTimer.hpp"
#include "../Animation.hpp"

#include "Paddle.hpp"
#include "PlayField.hpp"
#include "Brick.hpp"
#include "Missile.hpp"
#include "Blow.hpp"

#include <stack>


class Arkanoid final : public GameEngine::Game
{
private:

    enum class GameStage
    {
        START, IN_PROGRESS, GAMEOVER
    };

    static constexpr wchar_t const* const ASSETS_DIR{ L"data\\assets\\" };
    static constexpr wchar_t const* const ASSET_GAMESTART_IMG { L"gamestart.bmp" };
    static constexpr wchar_t const* const ASSET_GAMEOVER_IMG  { L"gameover.bmp" };
    static constexpr wchar_t const* const ASSET_MISSILE_SPRITE{ L"missile.bmp" };
    static constexpr wchar_t const* const ASSET_BLOW_ANIMATION{ L"blow.bmp" };

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr Rec2i WINDOW  { 0, 800, 600, 0 };
    static constexpr bool RESIZABLE{ false };

public:

    Arkanoid() = delete;
    Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&) = delete;

    ~Arkanoid() = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&) = delete;

    virtual void update() override;
    virtual void render() override;

private:

    void update_start_stage();
    void update_in_progress_stage(float dt);
    void update_gameover_stage();

    void render_full_scene();

    void kaboom(std::vector<std::shared_ptr<Missile>>::iterator const& missile, std::vector<std::vector<std::shared_ptr<Missile>>::iterator>& destroyed_missiles, std::stack<std::shared_ptr<Blow>>& blows_to_process);

    template<class F, class A1, class A2, template<class T, class A> class C1, template<class T, class A> class C2>
    static void delete_from_container(C1<std::shared_ptr<F>, A1>& container1, C2<typename C1<std::shared_ptr<F>, A1>::iterator, A2> const& container2)
    {
        for (auto const& item : container2)
        {
            std::remove_if(container1.begin(), container1.end(), [ptr=*item](std::shared_ptr<F> const& val) { return val == ptr; });
        }
        auto tmp{ container1.begin() };
        std::advance(tmp, container1.size() - container2.size());
        container1.erase(tmp, container1.end());
    }

private:
    
    static constexpr Rec2i PADDING{ 250, 30, 30, 30 };
    static constexpr Vec2i PADDLE_INIT_POS
    { 
        (WINDOW.get_width() - PADDING.left - PADDING.right) / 2 + PADDING.left,
        WINDOW.get_height() - PADDING.bottom - 60 
    };

    static constexpr int   PADDLE_INIT_HALF_WIDTH{  40   };
    static constexpr float PADDLE_INIT_SPEED     { 250.f };

    static constexpr int N_ROWS_BRICKS{ 5 };
    static constexpr GameEngine::Colour ROW_COLOURS[N_ROWS_BRICKS]
    { 
        GameEngine::Colours::INDIGO, 
        GameEngine::Colours::YELLOW,
        GameEngine::Colours::AZURE,
        GameEngine::Colours::STEEL_BLUE,
        GameEngine::Colours::CRIMSON 
    };
    static constexpr int N_BRICKS_IN_ROW{ (WINDOW.get_width() - PADDING.left - PADDING.right) / Brick::WIDTH };
    static constexpr int N_BRICKS_TOTAL { N_ROWS_BRICKS* N_BRICKS_IN_ROW };
    static constexpr Vec2i GRID_BRICKS_BEG
    { 
        PADDING.left + (WINDOW.get_width() - PADDING.left - PADDING.right) % Brick::WIDTH / 2,
        PADDING.top + 50
    };

    static constexpr Vec2f BALL_INIT_DIR{ 0.f, 1.f };
    static constexpr float BALL_INIT_SPEED{ 400.f };
    static constexpr Vec2i BALL_INIT_POS{ PADDLE_INIT_POS.x, PADDLE_INIT_POS.y - 100 };

    static constexpr float MISSILE_SPEED{ 300.f };

    GameStage cur_stage{ GameStage::START };

    PlayField field;
    Paddle    pad;
    GameEngine::FrameTimer ft{ };
    std::vector<Brick>     bricks;
    Ball      ball;

    GameEngine::Surface   gamestart_img;
    GameEngine::Surface   gameover_img;
    GameEngine::Surface   rocket;
    GameEngine::Animation blow_effect;

    std::vector<std::shared_ptr<Missile>> missiles;
    std::vector<std::shared_ptr<Blow>>    blows;
};