#pragma once

#include "ColourField.hpp"  

#include <unordered_map>
#include <iterator>
#include <map>
#include <queue>


template<GameEngine::Geometry::Rectangle2D<int> frame, GameEngine::Colour background_c>
class ColourGit final : public GameEngine::Interfaces::IDrawable
{
private:

    using id_commit = std::size_t;

    class Commit final
    {
    public: 

        Commit() = delete;

        constexpr Commit(ColourBlock& block, GameEngine::Colour old_c, GameEngine::Colour new_c)
        :
        block{ block },
        before_c{ old_c },
        after_c{ new_c }
        { 
            assert(old_c != new_c);
        }

        constexpr Commit(Commit const&) noexcept = default;
        constexpr Commit(Commit&&)      noexcept = default;

        Commit& operator=(Commit const&) = delete;
        Commit& operator=(Commit&&)      = delete;

        constexpr ~Commit() noexcept = default;


        void undo() const noexcept
        {
            block.set_colour(before_c);
        }

        void apply() const noexcept
        {
            block.set_colour(after_c);
        }

        constexpr ColourBlock& get_block() const noexcept
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
        static constexpr GameEngine::Geometry::Vector2D<int> DEFAULT_OFFSET{ 0, 0 };

    public:

                
        constexpr Branch(GameEngine::Geometry::Vector2D<int> const& init_offset = DEFAULT_OFFSET) noexcept
        :
        cur_offset{ init_offset }
        { }

        constexpr Branch(GameEngine::Geometry::Vector2D<int> const& init_offset, Branch const& parent, GameEngine::Colour parent_colour, id_commit head) noexcept
        :
        Branch{ init_offset }
        { 
            std::ranges::copy(parent.get_prev_transitions(), std::back_inserter(prev_transitions));
            prev_transitions.emplace_back(parent_colour, head);
            translation = parent.translation;
        }

        Branch(Branch const&)     = delete;
        constexpr Branch(Branch&&) noexcept = default;

        Branch& operator=(Branch const&) = delete;
        Branch& operator=(Branch&&)      = delete;

        constexpr ~Branch() noexcept = default;


        constexpr void commit(ColourBlock& block, GameEngine::Colour old_c, GameEngine::Colour new_c) noexcept
        {
            assert(old_c != new_c);
            commits.emplace_back((block.set_colour(new_c), block), old_c, new_c);
            polyline.emplace_back(cur_offset.x + translation.x, cur_offset.y + static_cast<int>(polyline.size()) * DISTANCE_BETWEEN_COMMITS + translation.y);
        }
         
        constexpr std::size_t get_n_commits() const noexcept
        {
            return commits.size();
        }

        constexpr id_commit get_last_commit_id() const noexcept
        {
            return get_n_commits() - 1U;
        }

        void translate_by(GameEngine::Geometry::Vector2D<int> const& delta) noexcept
        {
            for (auto& point : polyline)
            {
                point += delta;
            }
            translation += delta;
        }

        Commit& get_commit(id_commit i) noexcept
        {
            assert(i < commits.size());
            return commits[i];
        }

        constexpr Commit const& get_commit(id_commit i) const noexcept
        {
            assert(i < commits.size());
            return commits[i];
        }

        void set_offset(GameEngine::Geometry::Vector2D<int> const& new_offset) noexcept
        {
            for (auto const delta{ new_offset - cur_offset }; auto& point : polyline)
            {
                point += delta;
            }
            cur_offset = new_offset;
        }

        constexpr GameEngine::Geometry::Vector2D<int> get_offset() const noexcept
        {
            return cur_offset;
        }

        constexpr auto const& get_polyline() const noexcept
        {
            return polyline;
        }

        constexpr auto const& get_point(id_commit i) const noexcept
        {
            assert(i < polyline.size());
            return polyline[i];
        }
        
        constexpr auto const& get_prev_transitions() const noexcept
        {
            return prev_transitions;
        }

        constexpr std::optional<std::pair<GameEngine::Colour, id_commit>> get_parent() const noexcept
        {
            if (!prev_transitions.empty()) return prev_transitions.back();
            else                           return std::nullopt;
        }

        constexpr int get_side() const noexcept
        {
            return side;
        }

        void reverse_side() noexcept
        {
            side *= -1;
        }

        bool has_transit(GameEngine::Colour c) const noexcept
        {
            return std::ranges::any_of(prev_transitions, [c](auto const& pair){ return pair.first == c; });
        }

    private:

        std::vector<Commit> commits;

        GameEngine::Geometry::Vector2D<int> cur_offset;

        std::vector<std::pair<GameEngine::Colour, id_commit>> prev_transitions{ };

        GameEngine::Geometry::Vector2D<int> translation{ 0, 0 };
        std::vector<GameEngine::Geometry::Vector2D<int>> polyline{ };

        int side{ 1 };
    };

public:

    constexpr ColourGit() noexcept = default;
    constexpr ColourGit(std::size_t expected_n_branches) noexcept
    {
        branches.reserve(expected_n_branches);
    }
    ColourGit(ColourGit const&) = delete;
    ColourGit(ColourGit&&)      = delete;

    ColourGit& operator=(ColourGit const&) = delete;
    ColourGit& operator=(ColourGit&&)      = delete;

    constexpr virtual ~ColourGit() noexcept = default;

    constexpr void commit(ColourBlock& block, GameEngine::Colour new_c)
    {
        cur_branch->second.commit(block, block.get_colour(), new_c);
        head = cur_branch->second.get_last_commit_id();
    }

    constexpr void branch(GameEngine::Colour branch_c)
    {
        if (cur_branch == branches.end())
        {
            GameEngine::Geometry::Vector2D<int> const offset{ frame.left + frame.get_width() / 2, frame.top + Branch::DISTANCE_BETWEEN_COMMITS / 2 };

            cur_branch = branches.emplace(branch_c, offset).first;

            offsets_x.emplace(offset.x, branch_c);
        }
        else
        {
            cur_branch->second.reverse_side();

            GameEngine::Geometry::Vector2D<int> offset
            { 
                cur_branch->second.get_offset() + 
                GameEngine::Geometry::Vector2D<int>
                { 
                    Branch::DISTANCE_BETWEEN_COMMITS * cur_branch->second.get_side(),
                    Branch::DISTANCE_BETWEEN_COMMITS * static_cast<int>(head.value() + 1) 
                }
            };

            restruct_by_x(offset.x);

            cur_branch = branches.emplace
            (
                branch_c,
                Branch
                {
                    offset,
                    cur_branch->second,
                    cur_branch->first,
                    head.value()
                }
            ).first;

            offsets_x.emplace(offset.x, branch_c);
        }
    }

    constexpr ColourBlock& checkout(GameEngine::Colour branch_c)
    {
        assert(branches.contains(branch_c));

        auto begin_branch_trans{ cur_branch->second.get_prev_transitions() };
        auto end_branch_trans  { branches.at(branch_c).get_prev_transitions() };
        const auto[ begin_it, end_it ] = std::ranges::mismatch(begin_branch_trans, end_branch_trans);

        for (auto it{ begin_branch_trans.end() }; it != begin_it;)
        {
            rollback_cur_branch();
            --it;
            cur_branch = branches.find(it->first);
            head = it->second;
        }
        for (auto it{ end_it }; it != end_branch_trans.end(); ++it)
        {
            cur_branch = branches.find(it->first);
            head.reset();
            rollforward_cur_branch_to(it->second);
        }
        cur_branch = branches.find(branch_c);
        head.reset();
        rollforward_cur_branch_to(cur_branch->second.get_last_commit_id());

        return cur_branch->second.get_commit(head.value()).get_block();
    }

    constexpr void merge(GameEngine::Colour branch_c)
    {

    }

    void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
    {
        for (auto const& branch : branches)
        { 
            if (auto const parent{ branch.second.get_parent() }; parent.has_value() && !branch.second.get_polyline().empty())
                gfx.draw_line
                (
                    branches.at(parent.value().first).get_point(parent.value().second),
                    branch.second.get_point(0U),
                    BRANCH_LINE_THICKNESS,
                    branch.first,
                    frame
                );
        }

        for (auto const& branch : branches)
        {
            gfx.draw_line(branch.second.get_polyline().front(), branch.second.get_polyline().back(), BRANCH_LINE_THICKNESS, branch.first, frame);
           
            for (auto const& point : branch.second.get_polyline())
            {
                if (!frame.contains(point)) continue;

                gfx.fill_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, background_c);
                gfx.draw_ellipse(point, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_RADIUS, COMMIT_CIRCLE_BORDER_THICKNESS, branch.first);
            }
        }
        if (head.has_value() && frame.contains(cur_branch->second.get_point(head.value())))
            gfx.fill_ellipse
            (
                cur_branch->second.get_point(head.value()), 
                HEAD_CIRCLE_RADIUS,
                HEAD_CIRCLE_RADIUS,
                cur_branch->first
            );
    }

    constexpr GameEngine::Colour get_cur_branch() const
    {
        return cur_branch->first;
    }

    constexpr bool has_branch(GameEngine::Colour c) const noexcept
    {
        return branches.contains(c);
    }

    constexpr ColourBlock& rollback() noexcept
    {
        assert(head.has_value());

        if (head.value() != 0U) cur_branch->second.get_commit((head.value())--).undo();
        return cur_branch->second.get_commit(head.value()).get_block();
    }

    constexpr ColourBlock& rollforward() noexcept
    {
        assert(head.has_value());

        if (head.value() != cur_branch->second.get_last_commit_id()) cur_branch->second.get_commit(++(head.value())).apply();
        return cur_branch->second.get_commit(head.value()).get_block();
    }

    constexpr bool is_behind_head() const noexcept
    {
        assert(head.has_value());
        return head.value() < cur_branch->second.get_last_commit_id();
    }

    void move_in_frame(GameEngine::Geometry::Vector2D<int> delta)
    {
        for (auto& branch : branches)
        {
            branch.second.translate_by(delta);
        }
    }

    constexpr ColourBlock& move_to_head()
    {
        id_commit const last_commit_id{ cur_branch->second.get_last_commit_id() };
        rollforward_cur_branch_to(last_commit_id);
        return cur_branch->second.get_commit(last_commit_id).get_block();
    }

    std::optional<std::pair<GameEngine::Colour, ColourBlock*>> delete_branch(GameEngine::Colour branch_c)
    {
        assert(branches.contains(c));

        if (cur_branch->second.has_transit(branch_c))
        {
            do
            {
                rollback_cur_branch();
                auto const parent{ cur_branch->second.get_parent() };
                assert(parent.has_value());
                cur_branch = branches.find(parent->first);
                head = parent->second;
            } while (cur_branch->first != branch_c);
            rollback_cur_branch_to_parent();
        }
        else if (cur_branch->first == branch_c)
        {
            rollback_cur_branch_to_parent();
        }

        branches.erase(branches.find(branch_c));
        std::queue<GameEngine::Colour> to_erase{ { branch_c } };
        do
        {
            std::erase_if(branches, [&offsets_x = this->offsets_x, &to_erase, c{ to_erase.front() }](auto const& pair)
                {
                    if (pair.second.has_transit(c))
                    {
                        to_erase.push(pair.first);
                        offsets_x.erase(pair.second.get_offset().x);

                        return true;
                    }
                    return false;
                }
            );
            to_erase.pop();
        } while (!to_erase.empty());


        if (!head)
        {
            cur_branch = branches.end();
            return std::nullopt;
        }
        else return std::optional<std::pair<GameEngine::Colour, ColourBlock*>>{ std::pair<GameEngine::Colour, ColourBlock*>{ cur_branch->first, &(cur_branch->second.get_commit(head.value()).get_block()) } };
    }

private:

    constexpr void rollback_cur_branch() noexcept
    {
        assert(head.has_value());

        while (head.value() != 0U)
        {
            cur_branch->second.get_commit((head.value())--).undo();
        }
        cur_branch->second.get_commit(head.value()).undo();
        head.reset();
    }

    constexpr void rollforward_cur_branch_to(id_commit end) noexcept
    {
        if (!head.has_value())
        {
            head = 0U;
            cur_branch->second.get_commit(head.value()).apply();
        }

        assert(head.value() <= end);

        while (head.value() != end)
        {
            cur_branch->second.get_commit(++(head.value())).apply();
        } 
    }

    void restruct_by_x(int new_x)
    {
        if (auto const target{ offsets_x.find(new_x) }; target != offsets_x.end())
        {
            auto node{ offsets_x.extract(target) };
            do
            {
                node.key() += branches[node.mapped()].get_side() * Branch::DISTANCE_BETWEEN_COMMITS;
            } while (offsets_x.contains(node.key()));
            
            offsets_x.emplace(node.key(), node.mapped());
            
            auto new_offset{ branches[node.mapped()].get_offset()};
            new_offset.x = node.key();

            branches[node.mapped()].set_offset(new_offset);
        }
    }

    void rollback_cur_branch_to_parent() noexcept
    {
        rollback_cur_branch();

        if (auto const parent{ cur_branch->second.get_parent() }; parent)
        {
            cur_branch = branches.find(parent->first);
            head = parent->second;
        }
        else
            head.reset();
    }

private:

    static constexpr int COMMIT_CIRCLE_RADIUS{ 4 };
    static constexpr int HEAD_CIRCLE_RADIUS{ 6 };
    static constexpr int BRANCH_LINE_THICKNESS{ 2 };
    static constexpr int COMMIT_CIRCLE_BORDER_THICKNESS{ BRANCH_LINE_THICKNESS };

    std::unordered_map<GameEngine::Colour, Branch> branches{ };
    std::unordered_map<GameEngine::Colour, Branch>::iterator cur_branch{ branches.end() };
    std::optional<id_commit> head{ std::nullopt };

    std::map<int, GameEngine::Colour> offsets_x{ };
};