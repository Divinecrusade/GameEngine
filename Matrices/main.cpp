#include <iostream>

#include "UnitTests.hpp"


int main()
{
    std::ignore = UnitTests::is_pass_type_constraints_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_default_constructor_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_copy_constructor_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_copy_operator_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_parameter_constructor_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_swapping_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_iterator_test(std::clog, std::cerr);
    std::ignore = UnitTests::is_pass_views_test(std::clog, std::cerr);

    return EXIT_SUCCESS;
}