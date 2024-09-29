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

    Optionable(Option decision = DEFAULT) noexcept
    :
    decision{ decision }
    { }
    Optionable(Optionable const&) noexcept = default;
    Optionable(Optionable&&) noexcept      = default;

    Optionable& operator=(Optionable const&) noexcept = default;
    Optionable& operator=(Optionable&&) noexcept      = default;

    virtual ~Optionable() noexcept = default;
    

    void set_option(Option new_decision) noexcept
    {
        decision = new_decision;
    }

    Option get_option() const noexcept
    {
        return decision;
    }

    bool is_option_set() const noexcept
    {
        return decision != Option::NONE;
    }

private:

    Option decision{ Option::NONE };
};