#pragma once

#include "PulsatingBlock.hpp"
#include "Optionable.hpp"

#include <vector>


template<int SIZE>
requires (SIZE > 0)
class DuoColourBlock : public PulsatingBlock<SIZE>, public Optionable
{
public:

    constexpr DuoColourBlock(PulsationEffect const* pulsator, bool to_start_pulsation, GameEngine::Geometry::Vector2D<int> const& left_top_pos, GameEngine::Colour c1, GameEngine::Colour c2) noexcept
    :
    PulsatingBlock<SIZE>{ pulsator, to_start_pulsation, left_top_pos, c1 },
    c2{ c2 }
    {
        set_triangles(left_top_pos);
    }
    constexpr DuoColourBlock(PulsatingBlock<SIZE> const& block, bool to_start_pulsation, GameEngine::Colour c1, GameEngine::Colour c2)
    :
    PulsatingBlock<SIZE>{ block },
    c2{ c2 }
    { 
        this->set_colour(c1);
        if (to_start_pulsation)   this->pulsation_on();
        else                      this->pulsation_off();
        set_triangles({ PulsatingBlock<SIZE>::get_collision_box().left, PulsatingBlock<SIZE>::get_collision_box().top });
    }

    constexpr DuoColourBlock(DuoColourBlock const&) noexcept = default;
    constexpr DuoColourBlock(DuoColourBlock&&)      noexcept = default;

    constexpr DuoColourBlock& operator=(DuoColourBlock const&) noexcept = default;
    constexpr DuoColourBlock& operator=(DuoColourBlock&&)      noexcept = default;

    constexpr ~DuoColourBlock() noexcept = default;


    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        switch (get_option())
        {
            case Option::FIRST:
            {
                DrawableBlock<SIZE>::draw(gfx);
            }
            break;

            case Option::SECOND:
            {
                gfx.fill_rectangle(DrawableBlock<SIZE>::get_collision_box(), c2);
            }
            break;

            case Option::NONE:
            {
                gfx.fill_polygon(triangle1, (*PulsatingBlock<SIZE>::get_pulsator())(ColourBlock::get_colour()));
                gfx.fill_polygon(triangle2, (*PulsatingBlock<SIZE>::get_pulsator())(c2));
            }
            break;
        }
    }

    void choose_colour(GameEngine::Geometry::Vector2D<int> const& pos)
    {
        if (!DrawableBlock<SIZE>::get_collision_box().contains(pos)) return;

        int const ly{ -(pos.y - DrawableBlock<SIZE>::get_collision_box().top - SIZE / 2) };
        int const lx{ pos.x - DrawableBlock<SIZE>::get_collision_box().left - SIZE / 2 };

        if (ly > lx) this->set_option(Option::FIRST);
        else                                 this->set_option(Option::SECOND);
    }

private:

    void set_triangles(GameEngine::Geometry::Vector2D<int> const& left_top_pos)
    {
        triangle1.emplace_back(left_top_pos);
        triangle1.emplace_back(left_top_pos.x + SIZE, left_top_pos.y);
        triangle1.emplace_back(left_top_pos.x, left_top_pos.y + SIZE);

        triangle2.emplace_back(left_top_pos + GameEngine::Geometry::Vector2D<int>{ SIZE, SIZE });
        triangle2.emplace_back(triangle2[0U] + GameEngine::Geometry::Vector2D<int>{ -SIZE, 0 });
        triangle2.emplace_back(triangle2[0U] + GameEngine::Geometry::Vector2D<int>{ 0, -SIZE });
    }

private:

    std::vector<GameEngine::Geometry::Vector2D<int>> triangle1;
    std::vector<GameEngine::Geometry::Vector2D<int>> triangle2;
    GameEngine::Colour c2;   
};