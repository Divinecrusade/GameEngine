#pragma once

#include <string_view>
#include <string>
#include <sstream>


class AbstractUnitTest
{
public:

    [[nodiscard("Pure function")]] bool is_passed() noexcept
    {
    }

    virtual [[nodiscard("Pure function")]] std::string_view get_purpose() const noexcept = 0;
    virtual [[nodiscard("Pure function")]] std::string      get_report()  const noexcept
    {
        if (report.eof()) return ERR_TEST_NOT_EXECUTED.data();


    }

protected:

    virtual void execute() noexcept = 0;

private:

    static constexpr std::string_view ERR_TEST_NOT_EXECUTED{ "Test is not executed" };

    std::ostringstream report{ };
};
