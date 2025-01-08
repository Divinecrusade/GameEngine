#pragma once

#include <Matrix.hpp>

#include <iostream>
#include <iomanip>
#include <string_view>


namespace UnitTests
{
    namespace
    {
        static constexpr std::string_view SEPRATOR{ "|--------------------------------------------------------------------------|" };

        inline namespace StreamColors
        {
            static constexpr char const* RED{ "\033[31m" };
            static constexpr char const* GREEN{ "\033[32m" };
            static constexpr char const* RESET{ "\033[0m" };
        }

        static void print_test_name(std::ostream& log, std::string_view name)
        {
            log << SEPRATOR << "\n";
            log << "|" << std::setfill('-') << std::left << std::setw(SEPRATOR.length() - 2) << name << "|\n";
            log << SEPRATOR << "\n";
        }

        template <typename T, std::size_t M, std::size_t N>
        constexpr bool is_matrix_constructible()
        {
            return requires { typename Matrix<M, N, T>; };
        }

        template <typename T, std::size_t M, std::size_t N, bool is_constructible = is_matrix_constructible<T, M, N>()>
        static void check_invalid_type(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (is_constructible)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructible with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
            }
            log << "Type: " << typeid(T).name() << " " << M << " " << N << " Is constructible: " << (is_constructible ? "Yes" : "No") << '\n';
        }

        template <typename T, std::size_t M, std::size_t N, bool is_constructible = is_matrix_constructible<T, M, N>()>
        static void check_valid_type(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!is_constructible)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructible with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
            }
            log << "Type: " << typeid(T).name() << " " << M << " " << N << " Is constructible: " << (is_constructible ? "Yes" : "No") << '\n';
        }

        template <typename T, bool is_noexcept = noexcept(Matrix<3U, 3U, T>{}) >
        static void check_default_constructor(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!std::is_nothrow_default_constructible_v<T>)
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor marked noexcept but type is not nothrow default constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor not marked noexcept despite type being nothrow default constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Default constructor noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }

        template <typename T, bool is_noexcept = noexcept(Matrix<3U, 3U, T>{ std::declval<Matrix<3U, 3U, T>>() })>
        static void check_copy_constructor(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!noexcept(std::declval<T&>() = std::declval<T const&>()))
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor marked noexcept but type is not nothrow assignable:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor not marked noexcept despite type being nothrow assignable:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Copy constructor noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }

        template <typename T, bool is_noexcept = noexcept(std::declval<Matrix<3U, 3U, T>&>() = std::declval<Matrix<3U, 3U, T> const&>())>
        static void check_copy_operator(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!noexcept(std::declval<T&>() = std::declval<T const&>()))
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Copy operator marked noexcept but type is not nothrow assignable:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Copy operator not marked noexcept despite type being nothrow assignable:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Copy operator noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }
    }
    
    static bool is_pass_type_constraints_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

        print_test_name(log, "Matrix type constraints");

        check_invalid_type<void, 3U, 3U>(log, err, passed);
        check_invalid_type<std::nullptr_t, 3U, 3U>(log, err, passed);
        check_invalid_type<int*, 3U, 3U>(log, err, passed);
        check_invalid_type<std::string, 3U, 3U>(log, err, passed);
        check_invalid_type<int, 0U, 3U>(log, err, passed);
        check_invalid_type<int, 0U, 0U>(log, err, passed);
        check_invalid_type<int, 3U, 0U>(log, err, passed);

        check_valid_type<double, 3U, 1U>(log, err, passed);
        check_valid_type<double, 1U, 3U>(log, err, passed);
        check_valid_type<double, 1U, 1U>(log, err, passed);
        check_valid_type<float, 3U, 3U>(log, err, passed);
        check_valid_type<int, 3U, 3U>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix type constraints\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix type constraints\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_default_constructor_test(std::ostream& log, std::ostream& err) noexcept
    {
        bool passed{ true };

        print_test_name(log, "Matrix default constructor");

        check_default_constructor<int>(log, err, passed);
        check_default_constructor<double>(log, err, passed);
        check_default_constructor<float>(log, err, passed);
        check_default_constructor<char>(log, err, passed);
        check_default_constructor<unsigned>(log, err, passed);
        check_default_constructor<long double>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_constructor_test(std::ostream& log, std::ostream& err) noexcept
    {
        bool passed{ true };

        print_test_name(log, "Matrix copy constructor");

        check_copy_constructor<int>(log, err, passed);
        check_copy_constructor<double>(log, err, passed);
        check_copy_constructor<float>(log, err, passed);
        check_copy_constructor<char>(log, err, passed);
        check_copy_constructor<unsigned>(log, err, passed);
        check_copy_constructor<long double>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix copy constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix copy constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_operator_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

        print_test_name(log, "Matrix copy operator");

        check_copy_operator<int>(log, err, passed);
        check_copy_operator<double>(log, err, passed);
        check_copy_operator<float>(log, err, passed);
        check_copy_operator<char>(log, err, passed);
        check_copy_operator<unsigned>(log, err, passed);
        check_copy_operator<long double>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix copy operator\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix copy operator\n" << UnitTests::StreamColors::RESET;

        return passed;
    }
}