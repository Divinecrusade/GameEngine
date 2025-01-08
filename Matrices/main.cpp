#include <iostream>

#include "UnitTests.hpp"


int main()
{
    std::ignore = UnitTests::is_pass_construction_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_assignation_test(std::clog, std::cerr);

    return EXIT_SUCCESS;
}