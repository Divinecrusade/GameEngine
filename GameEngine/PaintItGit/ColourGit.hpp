#pragma once

#include "ColourField.hpp"  

#include <unordered_map>
#include <iterator>

template<typename field, GameEngine::Geometry::Rectangle2D<int> frame, GameEngine::Colour background_c>
class ColourGit final : public GameEngine::Interfaces::IDrawable
{
private:

    struct Commit final
    {
    public: 

        field::iterator loc;

        GameEngine::Colour const before_c;
        GameEngine::Colour const after_c;
    };

    class Branch final
    {
    public:

        static constexpr int DISTANCE_BETWEEN_COMMITS{ 20 };

    public:

        Branch(GameEngine::Geometry::Vector2D<int> const& init_offset = { 0, 0 })
        :
        cur_offset{ init_offset }
        { }

        std::size_t commit(field::iterator block, GameEngine::Colour old_c, GameEngine::Colour new_c)
        {
            commits.emplace_back((block->first = new_c, block), old_c, new_c);
            polyline.emplace_back(cur_offset.x, cur_offset.y + static_cast<int>(polyline.size()) * DISTANCE_BETWEEN_COMMITS);

            return commits.size() - 1U;
        }
         
        auto const& get_commits() const noexcept
        {
            return commits;
        }

        void set_offset(GameEngine::Geometry::Vector2D<int> const& new_offset)
        {
            for (auto const delta{ new_offset - cur_offset }; auto& point : polyline)
            {
                point += delta;
            }
            cur_offset = new_offset;
        }

        GameEngine::Geometry::Vector2D<int> get_offset() const noexcept
        {
            return cur_offset;
        }

        auto const& get_polyline() const noexcept
        {
            return polyline;
        }

    private:

        std::vector<Commit> commits;

        GameEngine::Geometry::Vector2D<int> cur_offset;
        std::vector<GameEngine::Geometry::Vector2D<int>> polyline{ };
    };

private:

    using branch_it = std::unordered_map<GameEngine::Colour, Branch>::iterator;

public:

    ColourGit() noexcept = default;
    ColourGit(ColourGit const&) = delete;
    ColourGit(ColourGit&&)      = delete;

    ColourGit& operator=(ColourGit const&) = delete;
    ColourGit& operator=(ColourGit&&)      = delete;

    virtual ~ColourGit() noexcept = default;

    void commit(field::iterator block, GameEngine::Colour new_c)
    {
        head = cur_branch->second.commit(block, block->first, new_c);
    }

    void branch(GameEngine::Colour branch_c)
    {
        if (cur_branch == branches.end())
        {
            cur_branch = branches.emplace(branch_c, GameEngine::Geometry::Vector2D<int>{ frame.left + frame.get_width() / 2, frame.top + Branch::DISTANCE_BETWEEN_COMMITS }).first;
        }
        else
        {
            branch_it const prev_branch
            { 
                std::exchange
                (
                    cur_branch, 
                    branches.emplace
                    (
                        branch_c, 
                        cur_branch->second.get_offset() + GameEngine::Geometry::Vector2D<int>{ Branch::DISTANCE_BETWEEN_COMMITS, Branch::DISTANCE_BETWEEN_COMMITS * static_cast<int>(head.value() + 1) }
                    ).first
                ) 
            };
        
            connections.emplace
            (
                cur_branch->first, 
                std::tuple<std::size_t, branch_it, std::size_t>{ 0U, prev_branch, *head }
            );
        }
    }

    void checkout(GameEngine::Colour branch_c)
    {
        /*auto const branch{branches.find(branch_c)};

        for (; cur_head != branch->get_parent(); --cur_head)
        {
            cur_head->loc.first = cur_head->before_c;
        }
        for (auto& commit : branch->get_commits())
        {
            commit->loc.first = commit->after_c;
        }
        cur_branch = branch;
        cur_head = std::prev(cur_branch->get_commits().end());*/
    }

    void merge(GameEngine::Colour branch_c)
    {

    }

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override
    {
        for (auto const& connection : connections)
        {
            gfx.draw_line
            (
                branches.at(connection.first).get_polyline()[std::get<0U>(connection.second)],
                std::get<1U>(connection.second)->second.get_polyline()[std::get<2U>(connection.second)],
                BRANCH_LINE_THICKNESS, 
                connection.first
            );
        }
        for (auto const& branch : branches)
        {
            gfx.draw_line(branch.second.get_polyline().front(), branch.second.get_polyline().back(), BRANCH_LINE_THICKNESS, branch.first);
            for (auto const& point : branch.second.get_polyline())
            {
                gfx.fill_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, background_c);
                gfx.draw_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_BORDER_THICKNESS, branch.first);
            }
        }
        if (head)
        {
            gfx.fill_ellipse
            (
                (cur_branch->second.get_polyline())[head.value()], 
                HEAD_CIRCLE_RADIUS,
                HEAD_CIRCLE_RADIUS,
                cur_branch->first
            );
        }
    }

    GameEngine::Colour get_cur_branch() const
    {
        return cur_branch->first;
    }

    bool has_branch(GameEngine::Colour c) const noexcept
    {
        return branches.contains(c);
    }

    field::iterator rollback() noexcept
    {
        std::vector<Commit> const& commits{ cur_branch->second.get_commits() };

        if (head && head.value() != 0U)
        {
            Commit commit{ commits[*head] };
            commit.loc->first = commit.before_c;
            --(*head);
        }
        return commits[*head].loc;
    }

    field::iterator rollforward() noexcept
    {
        std::vector<Commit> const& commits{ cur_branch->second.get_commits() };

        if (head && head.value() != commits.size() - 1U)
        {
            ++(*head);
            Commit commit{ commits[*head] };
            commit.loc->first = commit.after_c;
        }
        return commits[*head].loc;
    }

private:

    static constexpr int COMMIT_CIRCLE_RADIUS{ 4 };
    static constexpr int HEAD_CIRCLE_RADIUS{ 6 };
    static constexpr int BRANCH_LINE_THICKNESS{ 2 };
    static constexpr int COMMIT_CIRCLE_BORDER_THICKNESS{ BRANCH_LINE_THICKNESS };

    std::unordered_map<GameEngine::Colour, Branch> branches;
    branch_it cur_branch{ branches.end() };
    std::unordered_multimap<GameEngine::Colour, std::tuple<std::size_t, branch_it, std::size_t>> connections;
    std::optional<std::size_t> head{ std::nullopt };
};