#pragma once

#include "ColourField.hpp"  
#include "Optionable.hpp"

#include <unordered_map>
#include <iterator>
#include <map>
#include <queue>
#include <filesystem>
#include <fstream>
#include <functional>
#include <array>


template<GameEngine::Geometry::Rectangle2D<int> frame, GameEngine::Colour background_c, std::size_t N>
class ColourGit final : public GameEngine::Interfaces::IDrawable
{
public:

    static constexpr std::wstring_view DEFAULT_SAVE_FILE_URI{ L".\\colourgit.sav" };

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

        constexpr GameEngine::Colour get_c() const noexcept
        {
            return after_c;
        }

        constexpr GameEngine::Colour get_after() const noexcept
        {
            return after_c;
        }

        constexpr GameEngine::Colour get_before() const noexcept
        {
            return before_c;
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

        constexpr void translate_by(GameEngine::Geometry::Vector2D<int> const& delta) noexcept
        {
            for (auto& point : polyline)
            {
                point += delta;
            }
            translation += delta;
        }

        constexpr GameEngine::Geometry::Vector2D<int> get_translation() const noexcept
        {
            return translation;
        }

        constexpr std::vector<Commit>& get_commits() noexcept
        {
            return commits;
        }

        constexpr Commit& get_commit(id_commit i) noexcept
        {
            assert(i < commits.size());
            return commits[i];
        }

        constexpr Commit const& get_commit(id_commit i) const noexcept
        {
            assert(i < commits.size());
            return commits[i];
        }

        constexpr void set_offset(GameEngine::Geometry::Vector2D<int> const& new_offset) noexcept
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

        constexpr void reverse_side() noexcept
        {
            side *= -1;
        }

        constexpr bool has_transit(GameEngine::Colour c) const noexcept
        {
            return std::ranges::any_of(prev_transitions, [c](auto const& pair){ return pair.first == c; });
        }

        constexpr void mark_as_merged_to(GameEngine::Colour branch_c, id_commit branch_head) noexcept
        {
            assert(can_be_merged_to(branch_c));

            merged_to.emplace_back(std::make_tuple(get_last_commit_id(), branch_c, branch_head));
        }

        constexpr auto const& get_merged() const noexcept
        {
            return merged_to;
        }

        constexpr bool can_be_merged_to(GameEngine::Colour branch_c)
        {
            if (auto const it{ std::ranges::find_if(std::views::reverse(merged_to), [branch_c](auto const& tuple){ return std::get<1U>(tuple) == branch_c; })}; it == std::views::reverse(merged_to).end())
            {
                return true;
            }
            else
            {
                if (std::get<0U>(*it) == get_last_commit_id()) return false;
                else return true;
            }
        }

        constexpr auto const& get_merges() const noexcept
        {
            return merged_to;
        }

        constexpr void emplace_transition(GameEngine::Colour c, id_commit i)
        {
            prev_transitions.emplace_back(c, i);
        }

        constexpr void emplace_merge(id_commit i1, GameEngine::Colour c, id_commit i2)
        {
            merged_to.emplace_back(i1, c, i2);
        }

        constexpr void emplace_commit(ColourBlock& block, GameEngine::Colour c1, GameEngine::Colour c2)
        {
            commits.emplace_back(block, c1, c2);
            polyline.emplace_back(cur_offset.x + translation.x, cur_offset.y + static_cast<int>(polyline.size()) * DISTANCE_BETWEEN_COMMITS + translation.y);
        }

        constexpr std::optional<std::pair<id_commit, ColourBlock*>> move_to(GameEngine::Geometry::Vector2D<int> const& cursor_pos)
        {
            assert(!polyline.empty());

            if (cursor_pos.y < polyline.front().y || cursor_pos.y > polyline.back().y) return std::nullopt;

            size_t L{ 0U }, R{ polyline.size() }, k{ };
            while (L < R)
            {
                k = (L + R) / 2U;
                if (polyline[k].y - (DISTANCE_BETWEEN_COMMITS / 2) < cursor_pos.y && polyline[k].y + (DISTANCE_BETWEEN_COMMITS / 2) < cursor_pos.y) L = k + 1U;
                else R = k;
            }

            if (L == R) return std::make_pair(L, &commits[L].get_block());
            else return std::nullopt;
        }

    private:

        std::vector<Commit> commits;

        GameEngine::Geometry::Vector2D<int> cur_offset;

        std::vector<std::pair<GameEngine::Colour, id_commit>> prev_transitions{ };
        std::vector<std::tuple<id_commit, GameEngine::Colour, id_commit>> merged_to{ };

        GameEngine::Geometry::Vector2D<int> translation{ 0, 0 };
        std::vector<GameEngine::Geometry::Vector2D<int>> polyline{ };

        int side{ 1 };
    };

    class Conflict final : public Optionable
    {
    public:

        constexpr Conflict(ColourBlock& block, GameEngine::Colour first, GameEngine::Colour second)
        :
        Optionable{ },
        block{ block },
        first{ first },
        second{ second }
        { }


        constexpr GameEngine::Colour get(Option side) const noexcept
        {
            assert(side != Option::NONE);
            switch (side)
            {
                case Option::FIRST:  return first;
                case Option::SECOND: return second;
            }
        }

        constexpr GameEngine::Colour get_decision() const noexcept
        {
            assert(is_option_set());
            switch (get_option())
            {
                case Option::FIRST:  return first;
                case Option::SECOND: return second;
            }
        }

        constexpr ColourBlock& get_block() const noexcept
        {
            return block;
        }

    private:
        
        ColourBlock& block;

        GameEngine::Colour first;
        GameEngine::Colour second;
    };

public:

    constexpr ColourGit(std::function<std::size_t(ColourBlock&)> const& serializer,
                        std::function<ColourBlock&(std::size_t)> const& deserializer,
                        std::function<void(std::ofstream&)> const& saver,
                        std::function<void(std::ifstream&)> const& loader,
                        std::optional<std::size_t> expected_n_branches = std::nullopt, 
                        std::optional<std::filesystem::path> const& save_file_uri = std::nullopt)
    :
    serializer{ serializer },
    deserializer{ deserializer },
    saver{ saver },
    save_file_uri{ save_file_uri.value_or(std::filesystem::path{ DEFAULT_SAVE_FILE_URI }) }
    {
        if (!std::filesystem::exists(this->save_file_uri))
        {
            assert(init_state.has_value());

            if (expected_n_branches.has_value()) branches.reserve(expected_n_branches.value());
        }
        else
        {
            std::ifstream fin{ this->save_file_uri, std::ifstream::binary };

            loader(fin);

            bool has_head{ };
            GameEngine::Colour  head_c{ };
            id_commit           head_id{ };
            fin.read(reinterpret_cast<char*>(&has_head), sizeof(has_head));
            if (has_head)
            {
                fin.read(reinterpret_cast<char*>(&head_c.rgba), sizeof(head_c.rgba));
                fin.read(reinterpret_cast<char*>(&head_id), sizeof(head_id));
            }

            std::size_t n_branches{ };
            fin.read(reinterpret_cast<char*>(&n_branches), sizeof(n_branches));
            branches.reserve(n_branches);
            for (; n_branches != 0U; --n_branches)
            {
                GameEngine::Colour branch_c{ };
                int branch_offset_x{ };
                int branch_offset_y{ };

                fin.read(reinterpret_cast<char*>(&branch_c.rgba), sizeof(branch_c.rgba));
                fin.read(reinterpret_cast<char*>(&branch_offset_x), sizeof(branch_offset_x));
                fin.read(reinterpret_cast<char*>(&branch_offset_y), sizeof(branch_offset_y));

                emplace_branch(branch_c, { branch_offset_x, branch_offset_y });

                std::size_t transitions_n{ };
                fin.read(reinterpret_cast<char*>(&transitions_n), sizeof(transitions_n));
                for (; transitions_n != 0U; --transitions_n)
                {
                    GameEngine::Colour  transition_c{ };
                    id_commit          transition_id{ };

                    fin.read(reinterpret_cast<char*>(&transition_c.rgba), sizeof(transition_c.rgba));
                    fin.read(reinterpret_cast<char*>(&transition_id), sizeof(transition_id));

                    emplace_transition(branch_c, transition_c, transition_id);
                }

                std::size_t merges_n{ };
                fin.read(reinterpret_cast<char*>(&merges_n), sizeof(merges_n));
                for (; merges_n != 0U; --merges_n)
                {
                    id_commit         merge_id1{ };
                    GameEngine::Colour  merge_c{ };
                    id_commit         merge_id2{ };

                    fin.read(reinterpret_cast<char*>(&merge_id1), sizeof(merge_id1));
                    fin.read(reinterpret_cast<char*>(&merge_c.rgba), sizeof(merge_c.rgba));
                    fin.read(reinterpret_cast<char*>(&merge_id2), sizeof(merge_id2));

                    emplace_merge(branch_c, merge_id1, merge_c, merge_id2);
                }

                std::size_t n_commits{ };
                fin.read(reinterpret_cast<char*>(&n_commits), sizeof(n_commits));
                for (; n_commits != 0U; --n_commits)
                {
                    id_commit commit_id{ };
                    GameEngine::Colour commit_before{ };
                    GameEngine::Colour commit_after { };

                    fin.read(reinterpret_cast<char*>(&commit_id), sizeof(commit_id));
                    fin.read(reinterpret_cast<char*>(&commit_before.rgba), sizeof(commit_before.rgba));
                    fin.read(reinterpret_cast<char*>(&commit_after.rgba), sizeof(commit_after.rgba));

                    emplace_commit(branch_c, commit_id, commit_before, commit_after);
                }
            }

            if (has_head)
            {
                restore_head(head_c, head_id);
            }
        }
    }
    ColourGit(ColourGit const&) = delete;
    ColourGit(ColourGit&&)      = delete;

    ColourGit& operator=(ColourGit const&) = delete;
    ColourGit& operator=(ColourGit&&)      = delete;

    constexpr ~ColourGit() noexcept
    {
        std::ofstream fout{ save_file_uri, std::ofstream::binary };

        saver(fout);

        auto const b_tmp{head.has_value()};
        fout.write(reinterpret_cast<char const*>(&b_tmp), sizeof(b_tmp));
        if (head.has_value())
        {
            fout.write(reinterpret_cast<char const*>(&(cur_branch->first.rgba)), sizeof(cur_branch->first.rgba));
            fout.write(reinterpret_cast<char const*>(&(*head)), sizeof(*head));
        }

        auto tmp{ branches.size() };
        fout.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
        for (auto& [c, branch] : branches)
        {
            auto const tmp1{ branch.get_offset().x };
            auto const tmp2{ branch.get_offset().y };
            fout.write(reinterpret_cast<char const*>(&c.rgba), sizeof(c.rgba));
            fout.write(reinterpret_cast<char const*>(&tmp1), sizeof(tmp1));
            fout.write(reinterpret_cast<char const*>(&tmp2), sizeof(tmp2));
            
            tmp = branch.get_prev_transitions().size();
            fout.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));

            for (auto const& prev_transion : branch.get_prev_transitions())
            {
                fout.write(reinterpret_cast<char const*>(&(prev_transion.first.rgba)), sizeof(prev_transion.first.rgba));
                fout.write(reinterpret_cast<char const*>(&(prev_transion.second)), sizeof(prev_transion.second));
            }
            
            tmp = branch.get_merges().size();
            fout.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
            for (auto const& merge : branch.get_merges())
            {
                fout.write(reinterpret_cast<char const*>(&(std::get<0U>(merge))), sizeof(std::get<0U>(merge)));
                fout.write(reinterpret_cast<char const*>(&(std::get<1U>(merge))), sizeof(std::get<1U>(merge)));
                fout.write(reinterpret_cast<char const*>(&(std::get<2U>(merge))), sizeof(std::get<2U>(merge)));
            }

            tmp = branch.get_n_commits();
            fout.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
            for (auto& commit : branch.get_commits())
            {
                auto const tmp1{ serializer(commit.get_block()) };
                auto const tmp2{ commit.get_before().rgba };
                auto const tmp3{ commit.get_after().rgba };

                fout.write(reinterpret_cast<char const*>(&tmp1), sizeof(tmp1));
                fout.write(reinterpret_cast<char const*>(&tmp2), sizeof(tmp2));
                fout.write(reinterpret_cast<char const*>(&tmp3), sizeof(tmp3));
            }
        }
    }

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

    constexpr void draw(GameEngine::Interfaces::IGraphics2D& gfx, [[ maybe_unused ]] std::optional<GameEngine::Geometry::Rectangle2D<int>> const& = std::nullopt) const override
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

            for (auto const& merged : branch.second.get_merged())
            {
                gfx.draw_line
                (
                    branch.second.get_point(std::get<0U>(merged)),
                    branches.at(std::get<1U>(merged)).get_point(std::get<2U>(merged)),
                    BRANCH_LINE_THICKNESS,
                    branch.first,
                    frame
                );
            }
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

    constexpr std::optional<std::pair<GameEngine::Colour, ColourBlock*>> delete_branch(GameEngine::Colour branch_c)
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

        auto const branch_to_delete{ branches.find(branch_c) };
        offsets_x.erase(branch_to_delete->second.get_offset().x);
        branches.erase(branch_to_delete);
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

    constexpr bool prepare_merge(GameEngine::Colour branch_c)
    {
        assert(!prepared_merge.has_value());

        if (cur_branch->first == branch_c)
        {
            return false;
        }

        auto const to_merge{ branches.find(branch_c) };
        if (!to_merge->second.can_be_merged_to(cur_branch->first))
        {
            return false;
        }
        prepared_merge = branch_c;

        for (auto const& commit : to_merge->second.get_commits())
        {
            if (auto const it{ std::ranges::find_if(cur_branch->second.get_commits(), [&commit](auto const& val)
                {
                    return &val.get_block() == &commit.get_block();
                })}; it != cur_branch->second.get_commits().end())
                cur_conflicts.emplace_back(commit.get_block(), commit.get_c(), it->get_c());
            else
                this->commit(commit.get_block(), commit.get_c());
        }
        return true;
    }

    constexpr ColourBlock& merge()
    {
        assert(prepared_merge.has_value());

        auto const to_merge{ branches.find(prepared_merge.value()) };

        assert(to_merge->second.is_merged());

        for (auto const& conflict : cur_conflicts)
        {
            assert(conflict.is_option_set());
            this->commit(conflict.get_block(), conflict.get_decision());
        }
        to_merge->second.mark_as_merged_to(cur_branch->first, *head);

        cur_conflicts.clear();
        prepared_merge.reset();

        return cur_branch->second.get_commit(*head).get_block();
    }

    constexpr std::vector<Conflict>& get_conflicts() noexcept
    {
        return cur_conflicts;
    }

    constexpr std::optional<std::pair<GameEngine::Colour, ColourBlock*>> move_to(GameEngine::Geometry::Vector2D<int> cursor_pos)
    {
        assert(frame.contains(cursor_pos));
        assert(cur_branch != branches.end());

        static constexpr int BASE{ (frame.left + frame.get_width() / 2) % Branch::DISTANCE_BETWEEN_COMMITS };

        cursor_pos.x -= cur_branch->second.get_translation().x;

        decltype(offsets_x.begin()) it{ };
        if (int offset{ (cursor_pos.x) % Branch::DISTANCE_BETWEEN_COMMITS };
            (it = offsets_x.find(cursor_pos.x - offset + BASE)) != offsets_x.end()
            || (it = offsets_x.find(cursor_pos.x + (Branch::DISTANCE_BETWEEN_COMMITS - offset) + BASE)) != offsets_x.end())
        {
            auto const branch{ branches.find(it->second) };

            if (auto commit{ branch->second.move_to(cursor_pos) }; commit != std::nullopt)
            {
                 if (cur_branch == branch && head == commit->first) return std::nullopt;

                 if (cur_branch != branch) checkout(branch->first);
                 while (*head > commit->first)
                 {
                     rollback();
                 }
                 while (*head < commit->first)
                 {
                     rollforward();
                 }

                 return std::make_pair(branch->first, commit->second);
            }
            else return std::nullopt;
        }
        else return std::nullopt;
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

    constexpr void restruct_by_x(int new_x)
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

    constexpr void rollback_cur_branch_to_parent() noexcept
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

    constexpr void emplace_branch(GameEngine::Colour c, GameEngine::Geometry::Vector2D<int> const& offset)
    {
        branches.emplace(c, offset);
        offsets_x.emplace(offset.x, c);
    }

    constexpr void emplace_transition(GameEngine::Colour branch_c, GameEngine::Colour transition_c, id_commit transition_i)
    {
        branches[branch_c].emplace_transition(transition_c, transition_i);
    }

    constexpr void emplace_merge(GameEngine::Colour branch_c, id_commit i1, GameEngine::Colour c, id_commit i2)
    {
        branches[branch_c].emplace_merge(i1, c, i2);
    }

    constexpr void emplace_commit(GameEngine::Colour branch_c, id_commit i, GameEngine::Colour c1, GameEngine::Colour c2)
    {
        branches[branch_c].emplace_commit(deserializer(i), c1, c2);
    }

    constexpr void restore_head(GameEngine::Colour c, id_commit i)
    {
        cur_branch = std::ranges::find_if(branches, [](auto const& pair){ return pair.second.get_prev_transitions().empty(); });
        head = 0U;
        checkout(c);

        while (*head != i)
        {
            rollback();
        }
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

    std::optional<GameEngine::Colour> prepared_merge{ std::nullopt };
    std::vector<Conflict> cur_conflicts{ };

    std::function<std::size_t(ColourBlock&)> const serializer;
    std::function<ColourBlock&(std::size_t)> const deserializer;
    std::function<void(std::ofstream&)>      const saver;

    std::filesystem::path const save_file_uri;
};