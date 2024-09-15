#pragma once

#include "ColourField.hpp"  

#include <unordered_map>


template<typename field, GameEngine::Geometry::Rectangle2D<int> frame, GameEngine::Colour background_c>
class ColourGit final : public GameEngine::Interfaces::IDrawable
{
private:

    struct Commit final
    {
    public: 

        field::iterator const loc;

        GameEngine::Colour const before_c;
        GameEngine::Colour const after_c;
    };

    using pcommit = std::vector<Commit>::const_iterator;

    class Branch final
    {
    public:

        Branch(pcommit parent)
        :
        parent{ parent }
        { }

        void commit(field::iterator block, GameEngine::Colour old_c, GameEngine::Colour new_c)
        {
            commits.emplace_back(block, old_c, new_c);
            polyline.emplace_back(cur_offset.x, cur_offset.y + static_cast<int>(polyline.size()) * cur_distance_between_commits);
        }

        pcommit get_parent() const noexcept
        {
            return parent;
        }
         
        std::vector<pcommit const> const& get_commits() const noexcept
        {
            return commits;
        }

        void set_offset(GameEngine::Geometry::Vector2D<int> const& new_offset)
        {
            auto const delta{ new_offset - cur_offset };
            for (auto& point : polyline)
            {
                point += delta;
            }
            cur_offset = new_offset;
        }

        GameEngine::Geometry::Vector2D<int> get_offset() const noexcept
        {
            return cur_offset;
        }

        void set_distance_between_commits(int new_distance)
        {
            auto const delta{ new_distance - cur_distance_between_commits };
            for (auto& point : polyline)
            {
                point.y += delta;
            }
            cur_distance_between_commits = new_distance;
        }

        int get_distance_between_commits() const noexcept
        {
            return cur_distance_between_commits;
        }

        auto const& get_polyline() const noexcept
        {
            return polyline;
        }

    private:

        std::vector<Commit> commits;

        pcommit const parent;

        GameEngine::Geometry::Vector2D<int> cur_offset{ 0, 0 };
        int cur_distance_between_commits{ 0 };
        std::vector<GameEngine::Geometry::Vector2D<int>> polyline{ };
    };

public:

    ColourGit() noexcept = default;
    ColourGit(ColourGit const&) = delete;
    ColourGit(ColourGit&&)      = delete;

    ColourGit& operator=(ColourGit const&) = delete;
    ColourGit& operator=(ColourGit&&)      = delete;

    virtual ~ColourGit() noexcept = default;

    void commit(field::iterator block, GameEngine::Colour new_c)
    {
        cur_branch->second.commit(block, block->first, new_c);
    }

    void branch(GameEngine::Colour branch_c)
    {
        bool const no_parents{ cur_head == pcommit{ } };
        GameEngine::Geometry::Vector2D<int> offset{ };
        auto const prev_branch{ cur_branch };

        cur_branch = branches.emplace(branch_c, cur_head).first;

        if (no_parents)
        {
            cur_branch->second.set_offset(GameEngine::Geometry::Vector2D<int>{ frame.left + frame.get_width() / 2, frame.top + DISTANCE_BETWEEN_COMMITS });
            cur_branch->second.set_distance_between_commits(DISTANCE_BETWEEN_COMMITS);
        }
    }

    void checkout(GameEngine::Colour branch_c)
    {
        auto const branch{ branches.find(branch_c) };

        for (; cur_head != branch->get_parent(); --cur_head)
        {
            cur_head->loc.first = cur_head->before_c;
        }
        for (auto& commit : branch->get_commits())
        {
            commit->loc.first = commit->after_c;
        }
        cur_branch = branch;
        cur_head = std::prev(cur_branch->get_commits().end());
    }

    void merge(GameEngine::Colour branch_c)
    {

    }

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override
    {
        for (auto const& branch : branches)
        {
            gfx.draw_line(branch.second.get_polyline().front(), branch.second.get_polyline().back(), BRANCH_LINE_THICKNESS, branch.first);
            for (auto const& point : branch.second.get_polyline())
            {
                gfx.fill_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, background_c);
                gfx.draw_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_BORDER_THICKNESS, branch.first);
            }
        }
    }

private:

    static constexpr int COMMIT_CIRCLE_RADIUS{ 4 };
    static constexpr int BRANCH_LINE_THICKNESS{ 2 };
    static constexpr int COMMIT_CIRCLE_BORDER_THICKNESS{ BRANCH_LINE_THICKNESS };
    static constexpr int DISTANCE_BETWEEN_COMMITS{ 20 };

    std::unordered_map<GameEngine::Colour, Branch> branches;
    std::unordered_map<GameEngine::Colour, Branch>::iterator cur_branch{ branches.end() };
    pcommit cur_head{ };
};