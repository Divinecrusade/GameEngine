#pragma once

enum class Option
{
    FIRST, SECOND, NONE
};

class Optionable
{
public:

    static constexpr Option DEFAULT{ Option::NONE };

public:

    constexpr Optionable(Option decision = DEFAULT) noexcept
    :
    decision{ decision }
    { }
    constexpr Optionable(Optionable const&) noexcept = default;
    constexpr Optionable(Optionable&&) noexcept      = default;

    constexpr Optionable& operator=(Optionable const&) noexcept = default;
    constexpr Optionable& operator=(Optionable&&) noexcept      = default;

    constexpr virtual ~Optionable() noexcept = default;
    

    constexpr void set_option(Option new_decision) noexcept
    {
        decision = new_decision;
    }

    constexpr Option get_option() const noexcept
    {
        return decision;
    }

    constexpr bool is_option_set() const noexcept
    {
        return decision != Option::NONE;
    }

private:

    Option decision{ Option::NONE };
};