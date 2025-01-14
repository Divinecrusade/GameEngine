#include <iostream>

#include "UnitTests.hpp"


int main()
{
    std::size_t n_tests{ 0U };
    std::size_t n_failed{ 0U };

    ++n_tests, n_failed += !UnitTests::is_pass_type_constraints_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_default_constructor_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_copy_constructor_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_copy_operator_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_parameter_constructor_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_swapping_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_iterator_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_views_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_math_operations_test(std::clog, std::cerr);
    ++n_tests, n_failed += !UnitTests::is_pass_transformations_test(std::clog, std::cerr);

    std::clog << UnitTests::SEPARATOR << "\n";
    std::clog << "TESTS COMPLETED\n";
    std::clog << UnitTests::SEPARATOR << "\n";
    std::clog << "Total number of tests: " << n_tests << "\n";
    std::clog << "Failed:                " << UnitTests::StreamColors::RED << n_failed << UnitTests::StreamColors::RESET << "\n";
    std::clog << "Succeded:              " << UnitTests::StreamColors::GREEN << n_tests - n_failed << UnitTests::StreamColors::RESET << "\n";

    return EXIT_SUCCESS;
}