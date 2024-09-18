#pragma once

#include "ColourField.hpp"  

#include <unordered_map>
#include <iterator>


template<GameEngine::Geometry::Rectangle2D<int> frame, GameEngine::Colour background_c>
class ColourGit final : public GameEngine::Interfaces::IDrawable
{
private:

    using id_commit = std::size_t;

    class Commit final
    {
    public: 

        Commit(ColourBlock& block, GameEngine::Colour old_c, GameEngine::Colour new_c)
        :
        block{ block },
        before_c{ old_c },
        after_c{ new_c }
        { 
            assert(old_c != new_c);
        }

        void undo() const noexcept
        {
            block.set_colour(before_c);
        }

        void apply() const noexcept
        {
            block.set_colour(after_c);
        }

        ColourBlock& get_block() const noexcept
        {
            return block;
        }

    private:
        
        ColourBlock& block;

        GameEngine::Colour const before_c;
        GameEngine::Colour const after_c;
    };

    class Branch final
    {
    public:

        static constexpr int DISTANCE_BETWEEN_COMMITS{ 20 };

    public:

        Branch(GameEngine::Geometry::Vector2D<int> const& init_offset)
        :
        cur_offset{ init_offset }
        { }

        Branch(GameEngine::Geometry::Vector2D<int> const& init_offset, Branch const& parent, GameEngine::Colour parent_colour, id_commit head)
        :
        Branch{ init_offset }
        { 
            std::ranges::copy(parent.get_prev_transitions(), std::back_inserter(prev_transitions));
            prev_transitions.emplace_back(parent_colour, head);
        }

        void commit(ColourBlock& block, GameEngine::Colour old_c, GameEngine::Colour new_c)
        {
            assert(old_c != new_c);
            commits.emplace_back((block.set_colour(new_c), block), old_c, new_c);
            polyline.emplace_back(cur_offset.x, cur_offset.y + static_cast<int>(polyline.size()) * DISTANCE_BETWEEN_COMMITS);
        }
         
        std::size_t get_n_commits() const noexcept
        {
            return commits.size();
        }

        Commit const& get_commit(id_commit i) const noexcept
        {
            assert(i < commits.size());
            return commits[i];
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

        auto const& get_point(id_commit i) const noexcept
        {
            assert(i < polyline.size());
            return polyline[i];
        }
        
        std::vector<std::pair<GameEngine::Colour, id_commit>> const& get_prev_transitions() const
        {
            return prev_transitions;
        }

        std::optional<std::pair<GameEngine::Colour, id_commit>> get_parent() const
        {
            if (!prev_transitions.empty()) return prev_transitions.back();
            else                           return std::nullopt;
        }

    private:

        std::vector<Commit> commits;

        GameEngine::Geometry::Vector2D<int> cur_offset;

        std::vector<std::pair<GameEngine::Colour, id_commit>> prev_transitions{ };
        std::vector<GameEngine::Geometry::Vector2D<int>> polyline{ };
    };

public:

    ColourGit() noexcept = default;
    ColourGit(std::size_t expected_n_branches)
    {
        branches.reserve(expected_n_branches);
    }
    ColourGit(ColourGit const&) = delete;
    ColourGit(ColourGit&&)      = delete;

    ColourGit& operator=(ColourGit const&) = delete;
    ColourGit& operator=(ColourGit&&)      = delete;

    virtual ~ColourGit() noexcept = default;

    void commit(ColourBlock& block, GameEngine::Colour new_c)
    {
        cur_branch->second.commit(block, block.get_colour(), new_c);
        head = cur_branch->second.get_n_commits() - 1U;
    }

    void branch(GameEngine::Colour branch_c)
    {
        if (cur_branch == branches.end())
            cur_branch = branches.emplace
            (
                branch_c, 
                GameEngine::Geometry::Vector2D<int>
                { 
                    frame.left + frame.get_width() / 2, 
                    frame.top + Branch::DISTANCE_BETWEEN_COMMITS 
                }
            ).first;
        else
            cur_branch = branches.emplace
            (
                branch_c,
                Branch
                {
                    cur_branch->second.get_offset() + GameEngine::Geometry::Vector2D<int>{ Branch::DISTANCE_BETWEEN_COMMITS, Branch::DISTANCE_BETWEEN_COMMITS * static_cast<int>(head.value() + 1) },
                    cur_branch->second,
                    cur_branch->first,
                    head.value()
                }
            ).first;
    }

    void checkout(GameEngine::Colour branch_c)
    {
        return {};
    }

    void merge(GameEngine::Colour branch_c)
    {

    }

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, std::optional<GameEngine::Geometry::Rectangle2D<int>> const& clipping_area) const override
    {
        for (auto const& branch : branches)
        { 
            if (auto const parent{ branch.second.get_parent() }; parent.has_value() && !branch.second.get_polyline().empty())
                gfx.draw_line
                (
                    branches.at(parent.value().first).get_point(parent.value().second),
                    branch.second.get_point(0U),
                    BRANCH_LINE_THICKNESS,
                    branch.first
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
        if (head.has_value())
        {
            gfx.fill_ellipse
            (
                cur_branch->second.get_point(head.value()), 
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

    ColourBlock& rollback() noexcept
    {
        if (head.has_value() && head.value() != 0U) cur_branch->second.get_commit((head.value())--).undo();
        return cur_branch->second.get_commit(head.value()).get_block();
    }

    ColourBlock& rollforward() noexcept
    {
        if (head.has_value() && head.value() != cur_branch->second.get_n_commits() - 1U) cur_branch->second.get_commit(++(head.value())).apply();
        return cur_branch->second.get_commit(head.value()).get_block();
    }

private:

    static constexpr int COMMIT_CIRCLE_RADIUS{ 4 };
    static constexpr int HEAD_CIRCLE_RADIUS{ 6 };
    static constexpr int BRANCH_LINE_THICKNESS{ 2 };
    static constexpr int COMMIT_CIRCLE_BORDER_THICKNESS{ BRANCH_LINE_THICKNESS };

    std::unordered_map<GameEngine::Colour, Branch> branches{ };
    std::unordered_map<GameEngine::Colour, Branch>::iterator cur_branch{ branches.end() };
    std::optional<id_commit> head{ std::nullopt };
};