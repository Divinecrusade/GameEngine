#pragma once

#include <Matrix.hpp>

#include <iostream>
#include <iomanip>
#include <string_view>
#include <type_traits>


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

        class DummyExceptArithmetic
        {
        public:

            DummyExceptArithmetic() {};
            DummyExceptArithmetic(DummyExceptArithmetic const&) {};
            DummyExceptArithmetic(DummyExceptArithmetic&&) = delete;

            DummyExceptArithmetic& operator=(DummyExceptArithmetic const&) { return *this; }
            DummyExceptArithmetic& operator=(DummyExceptArithmetic&&) = delete;

            DummyExceptArithmetic operator+(DummyExceptArithmetic const&) const { return DummyExceptArithmetic{}; }
            DummyExceptArithmetic operator-(DummyExceptArithmetic const&) const { return DummyExceptArithmetic{}; }
            DummyExceptArithmetic operator*(DummyExceptArithmetic const&) const { return DummyExceptArithmetic{}; }
            DummyExceptArithmetic operator/(DummyExceptArithmetic const&) const { return DummyExceptArithmetic{}; }
        };

        static void print_test_name(std::ostream& log, std::string_view name)
        {
            log << SEPRATOR << "\n";
            log << "|" << std::setfill('-') << std::left << std::setw(SEPRATOR.length() - 2) << name << "|\n";
            log << SEPRATOR << "\n";
        }

        template<std::size_t M, std::size_t N, typename T>
        constexpr bool is_matrix_constructable()
        {
            return requires { typename Matrix<M, N, T>; };
        }

        template<std::size_t M, std::size_t N, typename T, typename... Args>
        constexpr bool is_matrix_constructable(Args&&... args)
        {
            return requires { Matrix<M, N, T>{ std::forward<Args>(args)... }; };
        }

        template<std::size_t M, std::size_t N, typename T, bool is_constructable = is_matrix_constructable<M, N, T>()>
        static void check_invalid_type(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (is_constructable)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructable with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
            }
            log << "Type: " << typeid(T).name() << " " << M << " " << N << " Is constructable: " << (is_constructable ? "Yes" : "No") << '\n';
        }

        template<std::size_t M, std::size_t N, typename T, bool is_constructable = is_matrix_constructable<M, N, T>()>
        static void check_valid_type(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!is_constructable)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructable with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
            }
            log << "Type: " << typeid(T).name() << " " << M << " " << N << " Is constructable: " << (is_constructable ? "Yes" : "No") << '\n';
        }

        template<std::size_t M, std::size_t N, typename T, bool is_noexcept = noexcept(Matrix<M, N, T>{}) >
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

        template<std::size_t M, std::size_t N, typename T, bool is_noexcept = noexcept(Matrix<M, N, T>{ std::declval<Matrix<M, N, T>>() })>
        static void check_copy_constructor(std::ostream& log, std::ostream& err, bool& passed)
        {
            if constexpr (!std::is_nothrow_copy_constructible_v<T>)
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Copy constructor marked noexcept but type is not nothrow constructable:\n" << StreamColors::RESET;
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

        template<std::size_t M, std::size_t N, typename T, bool is_noexcept = noexcept(std::declval<Matrix<M, N, T>&>() = std::declval<Matrix<M, N, T> const&>())>
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
    
        template<std::size_t M, std::size_t N, typename T, typename... Args>
        static void check_parameter_constructor_with_pack(std::ostream& log, std::ostream& err, bool& passed, Args&&... args)
        {
            bool is_constructable{ is_matrix_constructable<M, N, T>(std::forward<Args>(args)...) };

            if (is_constructable && sizeof...(Args) != M * N)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructable with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
            }
            log << "Matrix <" << M << ", " << N << "> {";
            ((log << " " << args), ...);
            log << " } Is constructable: " << (is_constructable ? "Yes" : "No") << '\n';
        }
    }
    
    static bool is_pass_type_constraints_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

        print_test_name(log, "Matrix type constraints");

        check_invalid_type<3U, 3U, void>(log, err, passed);
        check_invalid_type<3U, 3U, std::nullptr_t>(log, err, passed);
        check_invalid_type<3U, 3U, int*>(log, err, passed);
        check_invalid_type<3U, 3U, std::string>(log, err, passed);
        check_invalid_type<0U, 3U, int>(log, err, passed);
        check_invalid_type<0U, 0U, int>(log, err, passed);
        check_invalid_type<3U, 0U, int>(log, err, passed);

        check_valid_type<3U, 1U, double>(log, err, passed);
        check_valid_type<1U, 3U, double>(log, err, passed);
        check_valid_type<1U, 1U, double>(log, err, passed);
        check_valid_type<3U, 3U, float>(log, err, passed);
        check_valid_type<3U, 3U, int>(log, err, passed);
        check_valid_type<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix type constraints\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix type constraints\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_default_constructor_test(std::ostream& log, std::ostream& err) noexcept
    {
        bool passed{ true };

        print_test_name(log, "Matrix default constructor");

        check_default_constructor<3U, 3U, int>(log, err, passed);
        check_default_constructor<3U, 3U, double>(log, err, passed);
        check_default_constructor<3U, 3U, float>(log, err, passed);
        check_default_constructor<3U, 3U, char>(log, err, passed);
        check_default_constructor<3U, 3U, unsigned>(log, err, passed);
        check_default_constructor<3U, 3U, long double>(log, err, passed);
        check_default_constructor<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_constructor_test(std::ostream& log, std::ostream& err) noexcept
    {
        bool passed{ true };

        print_test_name(log, "Matrix copy constructor");
        check_copy_constructor<3U, 3U, int>(log, err, passed);
        check_copy_constructor<3U, 3U, double>(log, err, passed);
        check_copy_constructor<3U, 3U, float>(log, err, passed);
        check_copy_constructor<3U, 3U, char>(log, err, passed);
        check_copy_constructor<3U, 3U, unsigned>(log, err, passed);
        check_copy_constructor<3U, 3U, long double>(log, err, passed);
        check_copy_constructor<3U, 3U, long double>(log, err, passed);
        check_copy_constructor<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix copy constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix copy constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_operator_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

        print_test_name(log, "Matrix copy operator");

        check_copy_operator<3U, 3U, int>(log, err, passed);
        check_copy_operator<3U, 3U, double>(log, err, passed);
        check_copy_operator<3U, 3U, float>(log, err, passed);
        check_copy_operator<3U, 3U, char>(log, err, passed);
        check_copy_operator<3U, 3U, unsigned>(log, err, passed);
        check_copy_operator<3U, 3U, long double>(log, err, passed);
        check_copy_operator<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix copy operator\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix copy operator\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_parameter_constructor_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

        print_test_name(log, "Matrix parameter constructor");

        check_parameter_constructor_with_pack<1U, 1U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<1U, 0U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<0U, 0U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<1U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 1U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3., 4., 5.);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix parameter constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix parameter constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }
}