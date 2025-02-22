#pragma once

#include <Matrix.hpp>

#include <iostream>
#include <iomanip>
#include <string_view>
#include <type_traits>
#include <vector>


namespace UnitTests
{
    using namespace GameEngine::Geometry::Matrices;

    namespace
    {
        static constexpr std::string_view SEPARATOR{ "|--------------------------------------------------------------------------|" };

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

            friend bool operator==(DummyExceptArithmetic const&, DummyExceptArithmetic const&) { return true; }
            friend bool operator!=(DummyExceptArithmetic const&, DummyExceptArithmetic const&) { return false; }
        };
        static_assert(arithmetic_like<DummyExceptArithmetic>);

        std::ostream& operator<<(std::ostream& out, DummyExceptArithmetic const&)
        {
            out << "Dummy";
            return out;
        }


        static void print_test_name(std::ostream& log, std::string_view name)
        {
            log << SEPARATOR << "\n";
            log << "|" << std::setfill('-') << std::left << std::setw(SEPARATOR.length() - 2) << name << std::setfill(' ') << "|\n";
            log << SEPARATOR << "\n";
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
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
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
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Copy operator noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }
    
        template<std::size_t M, std::size_t N, typename T, typename... Args>
        static void check_parameter_constructor_with_pack(std::ostream& log, std::ostream& err, bool& passed, Args&&... args)
        {
            bool const is_constructable{ is_matrix_constructable<M, N, T>(std::forward<Args>(args)...) };

            if (is_constructable && sizeof...(Args) != M * N)
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix should not be constructable with this type:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
            }
            log << "Matrix <" << M << ", " << N << "> {";
            ((log << " " << args), ...);
            log << " } Is constructable: " << (is_constructable ? "Yes" : "No") << '\n';
        }

        template<std::size_t M, std::size_t N, typename T, typename... Args>
        static void check_parameter_constructor_with_pack_noexcept(std::ostream& log, std::ostream& err, bool& passed, Args&&...)
        {
            constexpr bool is_noexcept{ std::is_nothrow_constructible_v<Matrix<M, N, T>, Args...> };
            if constexpr (!(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>))
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor marked noexcept but type is not nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor not marked noexcept despite type being nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Parameter constructor (pack) noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }
    
        template<std::size_t M, std::size_t N, typename T>
        static void check_parameter_constructor_with_1d_array_noexcept(std::ostream& log, std::ostream& err, bool& passed, std::array<T, M * N>)
        {
            constexpr bool is_noexcept{ std::is_nothrow_constructible_v<Matrix<M, N, T>, std::array<T, M * N>> };
            if constexpr (!(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>))
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor marked noexcept but type is not nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor not marked noexcept despite type being nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Parameter constructor (1d array) noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }
    
        template<std::size_t M, std::size_t N, typename T>
        static void check_parameter_constructor_with_2d_array_noexcept(std::ostream& log, std::ostream& err, bool& passed, std::array<std::array<T, N>, M>)
        {
            constexpr bool is_noexcept{ std::is_nothrow_constructible_v<Matrix<M, N, T>, std::array<std::array<T, N>, M>> };
            if constexpr (!(std::is_move_constructible_v<T> || std::is_nothrow_constructible_v<T>))
            {
                if (is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor marked noexcept but type is not nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            else
            {
                if (!is_noexcept)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Constructor not marked noexcept despite type being nothrow copy constructible:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
            }
            log << "Type: " << typeid(T).name() << " Copy constructor (2d array) noexcept: " << (is_noexcept ? "Yes" : "No") << '\n';
        }
    
        static constexpr auto print_range
        {
            [](auto const& r, std::ostream& out) noexcept
            {
                for (auto const& el : r)
                {
                    out << " " << el;
                }
            }
        };

        template<std::size_t M, std::size_t N, typename T>
        static void print_matrix(Matrix<M, N, T> const& m, std::ostream& out, std::size_t setl = 8)
        {
            for (std::size_t i{ 0U }; i != M; ++i)
            {
                for (std::size_t j{ 0U }; j != N; ++j)
                {
                    out << std::setw(setl) << m[i][j];
                }
                out << '\n';
            }
        }

        template<std::size_t M, std::size_t N, typename T, typename R>
        static void check_matrix_and_range(std::ostream& log, std::ostream& err, bool& passed, Matrix<M, N, T> const& m, R const& r)
        {
            bool equal{ true };

            for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS && equal; ++i)
            for (std::size_t j{ 0U }; j != m.NUMBER_OF_COLS && equal; ++j)
            {
                auto it{ r.begin() };
                std::advance(it, i * m.NUMBER_OF_COLS + j);
                if (*it != m[i][j]) equal = false;
            }

            if (equal)
            {
                log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            }
            else
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix and range not equal:\n" << StreamColors::RESET;
            }
            log << "Matrix {";
            for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS && equal; ++i)
                print_range(m[i], log);
            log << " } and range {";
            print_range(r, log);
            log << " }\n";
        }
    
        template<std::size_t M, std::size_t N, typename T, typename R>
        static void check_row_and_range(std::ostream& log, std::ostream& err, bool& passed, Matrix<M, N, T> const& m, std::size_t i, R const& r)
        {
            bool equal{ true };

            for (std::size_t j{ 0U }; j != m.NUMBER_OF_COLS && equal; ++j)
            {
                auto it{ r.begin() };
                std::advance(it, j);
                if (*it != m[i][j]) equal = false;
            }

            if (equal)
            {
                log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            }
            else
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix row and range not equal:\n" << StreamColors::RESET;
            }
            log << "Matrix row {";
            print_range(m[i], log);
            log << " } and range {";
            print_range(r, log);
            log << " }\n";
        }
    
        template<std::size_t M, std::size_t N, typename T, typename R>
        static void check_col_and_range(std::ostream& log, std::ostream& err, bool& passed, Matrix<M, N, T> const& m, std::size_t j, R const& r)
        {
            bool equal{ true };

            for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS && equal; ++i)
            {
                auto it{ r.begin() };
                std::advance(it, i);
                if (*it != m[i][j]) equal = false;
            }

            if (equal)
            {
                log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            }
            else
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Matrix col and range not equal:\n" << StreamColors::RESET;
            }
            log << "Matrix col {";
            for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
            {
                log << " " << m[i][j];
            }
            log << " } and range {";
            print_range(r, log);
            log << " }\n";
        }
    }
    
    static bool is_pass_type_constraints_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix type constraints" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

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

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_default_constructor_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix default constructor" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        check_default_constructor<3U, 3U, int>(log, err, passed);
        check_default_constructor<3U, 3U, double>(log, err, passed);
        check_default_constructor<3U, 3U, float>(log, err, passed);
        check_default_constructor<3U, 3U, char>(log, err, passed);
        check_default_constructor<3U, 3U, unsigned>(log, err, passed);
        check_default_constructor<3U, 3U, long double>(log, err, passed);
        check_default_constructor<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_constructor_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix copy constructor" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);
        check_copy_constructor<3U, 3U, int>(log, err, passed);
        check_copy_constructor<3U, 3U, double>(log, err, passed);
        check_copy_constructor<3U, 3U, float>(log, err, passed);
        check_copy_constructor<3U, 3U, char>(log, err, passed);
        check_copy_constructor<3U, 3U, unsigned>(log, err, passed);
        check_copy_constructor<3U, 3U, long double>(log, err, passed);
        check_copy_constructor<3U, 3U, long double>(log, err, passed);
        check_copy_constructor<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_copy_operator_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix copy operator" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        check_copy_operator<3U, 3U, int>(log, err, passed);
        check_copy_operator<3U, 3U, double>(log, err, passed);
        check_copy_operator<3U, 3U, float>(log, err, passed);
        check_copy_operator<3U, 3U, char>(log, err, passed);
        check_copy_operator<3U, 3U, unsigned>(log, err, passed);
        check_copy_operator<3U, 3U, long double>(log, err, passed);
        check_copy_operator<3U, 3U, DummyExceptArithmetic>(log, err, passed);

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_parameter_constructor_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix parameter constructor" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        check_parameter_constructor_with_pack<1U, 1U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<1U, 0U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<0U, 0U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<1U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 1U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3.);
        check_parameter_constructor_with_pack<2U, 2U, double>(log, err, passed, 2., 3., 4., 5.);
        check_parameter_constructor_with_pack<1U, 1U, DummyExceptArithmetic>(log, err, passed, DummyExceptArithmetic{});

        check_parameter_constructor_with_pack_noexcept<2U, 2U, double>(log, err, passed, 2., 3., 4., 5.);
        check_parameter_constructor_with_pack_noexcept<1U, 1U, int>(log, err, passed, 2);
        check_parameter_constructor_with_pack_noexcept<1U, 1U, DummyExceptArithmetic>(log, err, passed, DummyExceptArithmetic{});

        check_parameter_constructor_with_1d_array_noexcept<2U, 1U, double>(log, err, passed, {{2., 1.}});
        check_parameter_constructor_with_1d_array_noexcept<2U, 1U, int>(log, err, passed, {{2, 1}});
        check_parameter_constructor_with_1d_array_noexcept<1U, 1U, DummyExceptArithmetic>(log, err, passed, { DummyExceptArithmetic{} });

        check_parameter_constructor_with_2d_array_noexcept<2U, 1U, double>(log, err, passed, {{ {2.}, {1.} }});
        check_parameter_constructor_with_2d_array_noexcept<2U, 2U, int>(log, err, passed, {{ {2, 1} }});
        check_parameter_constructor_with_2d_array_noexcept<1U, 2U, DummyExceptArithmetic>(log, err, passed, { {DummyExceptArithmetic{}} });

        {
            std::vector<int> const v{ 0, 1, 2 };
            static_assert(std::constructible_from<Matrix<3U, 1U, int>, decltype(v)> == true);
            Matrix<3U, 1U, int> const m1{ std::views::all(v) };
            Matrix<1U, 3U, int> const m2{ v };

            log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            log << "Matrix constructed from vector {";
            print_range(v, log);
            log << " }\n";

            check_matrix_and_range(log, err, passed, m1, v);
        }
        {
            std::vector<float> const v{ -2.f, 3.f, 2.f, 2.f, 4.f, 5.f };
            static_assert(std::constructible_from<Matrix<3U, 1U, int>, decltype(v)> == false);
            Matrix<3U, 1U, float> const m1{ std::views::reverse(v) | std::views::take(3) };
            log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            log << "Matrix constructed from part of vector {";
            print_range(std::views::reverse(v) | std::views::take(3), log);
            log << " }\n";
            check_matrix_and_range(log, err, passed, m1, std::views::reverse(v) | std::views::take(3));

            Matrix<1U, 3U, float> const m2{ v | std::views::take(3) };
            log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            log << "Matrix constructed from part of vector {";
            print_range(std::views::take(v, 3), log);
            log << " }\n";
            check_matrix_and_range(log, err, passed, m2, std::views::take(v, 3));

            constexpr std::array<int, 9U> a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            constexpr Matrix<3U, 3U, int> m3{ a };
            log << UnitTests::StreamColors::GREEN << "[OK] " << UnitTests::StreamColors::RESET;
            log << "Matrix constexpr constructed from constexpr array {";
            print_range(a, log);
            log << " }\n";
            check_matrix_and_range(log, err, passed, m3, a);

            log << "Flattern checks beg (row access is comparing with flattern access):\n";
            check_matrix_and_range(log, err, passed, m1, m1.flattern());
            check_matrix_and_range(log, err, passed, m2, m2.flattern());
            check_matrix_and_range(log, err, passed, m3, m3.flattern());
            log << "Flattern checks end\n";
        }

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_swapping_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Columns and rows swapping" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        {
            constexpr std::array row0{ 1, 2, 3 };
            constexpr std::array row1{ 4, 5, 6 };
            constexpr std::array row2{ 7, 8, 9 };

            constexpr std::array matrix{ row0, row1, row2 };
            Matrix<3U, 3U, int> m{ matrix };
            check_matrix_and_range(log, err, passed, m, std::views::join(matrix));

            log << "Init matrix:\n";
            print_matrix(m, log);

            std::size_t lhs{ 0U };
            std::size_t rhs{ 2U };
            m.swap_rows(lhs, rhs);

            log << "Make swap rows of " << lhs << " and " << rhs << ":\n";
            print_matrix(m, log);
            check_row_and_range(log, err, passed, m, lhs, row2);
            check_row_and_range(log, err, passed, m, rhs, row0);

            lhs = 1U;
            rhs = 2U;
            m.swap_rows(lhs, rhs);

            log << "Make swap rows of " << lhs << " and " << rhs << ":\n";
            print_matrix(m, log);
            check_row_and_range(log, err, passed, m, lhs, row0);
            check_row_and_range(log, err, passed, m, rhs, row1);

            m.swap_cols(lhs, rhs);

            log << "Make swap cols of " << lhs << " and " << rhs << ":\n";
            print_matrix(m, log);
            check_col_and_range(log, err, passed, m, lhs, std::vector<int>{9, 3, 6});
            check_col_and_range(log, err, passed, m, rhs, std::vector<int>{8, 2, 5});

            lhs = 0U;
            rhs = 1U;
            m.swap_cols(lhs, rhs);

            log << "Make swap cols of " << lhs << " and " << rhs << ":\n";
            print_matrix(m, log);
            check_col_and_range(log, err, passed, m, lhs, std::vector<int>{9, 3, 6});
            check_col_and_range(log, err, passed, m, rhs, std::vector<int>{7, 1, 4});
        }

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_iterator_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix iterating" };
        bool passed{ true };

        Matrix<3U, 3U, int> m{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

        print_test_name(log, TEST_NAME);

        log << "Iterator based loop begin\n";
        for (auto it{ m.begin() }; it != m.end(); ++it)
        {
            if (m[it->get_row_index()][it->get_col_index()] != it->get_data())
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
            }
            log << " [" << it->get_row_index() << "][" << it->get_col_index() << "]: " << it->get_data() << " && " << m[it->get_row_index()][it->get_col_index()] << "\n";
        }
        log << "Iterator based loop end\n";

        log << "Range based loop begin\n";
        for (auto const& el : m)
        {
            if (m[el.get_row_index()][el.get_col_index()] != el.get_data())
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Range-iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
            }
            log << " [" << el.get_row_index() << "][" << el.get_col_index() << "]:" << el.get_data() << " && " << m[el.get_row_index()][el.get_col_index()] << "\n";
        }
        log << "Range based loop end\n";

        print_matrix(m, log);

        log << "Iterator arithmetic begin\n";

        if ((m.begin() + 3)->get_data() != m[1U][0U])
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
        }
        else if ((m.begin() + 3)->get_row_index() != 1U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong row index:\n" << StreamColors::RESET;
        }
        else if ((m.begin() + 3)->get_col_index() != 0U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong column index:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "[" << (m.begin() + 3)->get_row_index() << "][" << (m.begin() + 3)->get_col_index() << "]:" << (m.begin() + 3)->get_data() << " && " << m[1U][0U] << " (begin() + 3 && [1][0])\n";

        auto it{ m.begin() + 3 };
        std::advance(it, 4);
        if (it->get_data() != m[2U][1U])
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
        }
        else if (it->get_row_index() != 2U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong row index:\n" << StreamColors::RESET;
        }
        else if (it->get_col_index() != 1U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong column index:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "[" << it->get_row_index() << "][" << it->get_col_index() << "]:" << it->get_data() << " && " << m[2U][1U] << " (advance(begin() + 3, 4) && [2][1])\n";

        ++it;
        if (it->get_data() != m[2U][2U])
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
        }
        else if (it->get_row_index() != 2U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong row index:\n" << StreamColors::RESET;
        }
        else if (it->get_col_index() != 2U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong column index:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "[" << it->get_row_index() << "][" << it->get_col_index() << "]:" << it->get_data() << " && " << m[2U][2U] << " ++(advance(begin() + 3, 4) && [2][2])\n";

        it -= 2;
        if (it->get_data() != m[2U][0U])
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
        }
        else if (it->get_row_index() != 2U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong row index:\n" << StreamColors::RESET;
        }
        else if (it->get_col_index() != 0U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong column index:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "[" << it->get_row_index() << "][" << it->get_col_index() << "]:" << it->get_data() << " && " << m[2U][0U] << " ++(advance(begin() + 3, 4) -= 2 && [2][0])\n";

        --it;
        if (it->get_data() != m[1U][2U])
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
        }
        else if (it->get_row_index() != 1U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong row index:\n" << StreamColors::RESET;
        }
        else if (it->get_col_index() != 2U)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Iterator calculated wrong column index:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "[" << it->get_row_index() << "][" << it->get_col_index() << "]:" << it->get_data() << " && " << m[1U][2U] << " --(++(advance(begin() + 3, 4) -= 2) && [1][2])\n";

        log << "Iterator arithmetic end\n";

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_views_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix views" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        Matrix<3U, 3U, int> m1{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        constexpr Matrix<3U, 2U, int> m2{ 0, 1, 2, 3, 4, 5 };
        constexpr Matrix<2U, 3U, int> m3{ 0, 1, 2, 3, 4, 5 };
        
        auto const check_matrices_row_views
        {
            [&passed, &log, &err](auto const& m)
            {
                log << "Matrix " << m.NUMBER_OF_ROWS << "x" << m.NUMBER_OF_COLS << ":\n";

                for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
                {
                    auto const v{ m.get_row(i) };
                    if (*v.get_type() != TypeOfMatrixView::ROW)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View row has not type ROW\n" << StreamColors::RESET;
                    }
                    else if (*v.get_index() != i)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View row has wrong index\n" << StreamColors::RESET;
                    }
                    if constexpr (v.NUMBER_OF_ELEMENTS != m.NUMBER_OF_COLS)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View row has wrong number of elements\n" << StreamColors::RESET;
                    }

                    for (std::size_t j{ 0U }; j != m.NUMBER_OF_COLS && passed; ++j)
                    {
                        if (m[i][j] != v[j])
                        {
                            passed = false;
                            err << StreamColors::RED << "[ERROR] View has wrong value\n" << StreamColors::RESET;
                        }
                        else
                        {
                            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                        }
                        log << "[" << *v.get_index() << "][" << j << "]:" << v[j] << " && " << m[i][j] << "\n";
                    }
                }
            }
        };
        auto const check_matrices_col_views
        {
            [&passed, &log, &err](auto const& m)
            {
                log << "Matrix " << m.NUMBER_OF_ROWS << "x" << m.NUMBER_OF_COLS << ":\n";

                for (std::size_t i{ 0U }; i != m.NUMBER_OF_COLS; ++i)
                {
                    auto const v{ m.get_col(i) };
                    if (*v.get_type() != TypeOfMatrixView::COLUMN)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View column has not type COLUMN\n" << StreamColors::RESET;
                    }
                    else if (*v.get_index() != i)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View column has wrong index\n" << StreamColors::RESET;
                    }
                    if constexpr (v.NUMBER_OF_ELEMENTS != m.NUMBER_OF_ROWS)
                    {
                        passed = false;
                        err << StreamColors::RED << "[ERROR] View column has wrong number of elements\n" << StreamColors::RESET;
                    }

                    for (std::size_t j{ 0U }; j != m.NUMBER_OF_ROWS && passed; ++j)
                    {
                        if (m[j][i] != v[j])
                        {
                            passed = false;
                            err << StreamColors::RED << "[ERROR] View has wrong value\n" << StreamColors::RESET;
                        }
                        else
                        {
                            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                        }
                        log << "[" << i << "][" << *v.get_index() << "]:" << v[j] << " && " << m[j][i] << "\n";
                    }
                }
            }
        };

        log << "Row views begin\n";
        check_matrices_row_views(m1);
        check_matrices_row_views(m2);
        check_matrices_row_views(m3);
        log << "Row views end\n";

        log << "Column views begin\n";
        check_matrices_col_views(m1);
        check_matrices_col_views(m2);
        check_matrices_col_views(m3);
        log << "Column views end\n";

        auto m{ m1 };

        log << "Range based loop (row) begin\n";

        log << "Using matrix:\n";
        print_matrix(m, log);

        for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
        {
            std::size_t j{ 0U };
            for (auto const& el : m.get_row(i))
            {
                if (m[i][j] != el)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Range-iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
                log << " [" << i << "][" << j << "]:" << el << " && " << m[i][j] << "\n";
                ++j;
            }
        }
        log << "Range based loop (row) end\n";

        m = std::move(m1);

        log << "Range based loop (column) begin\n";

        log << "Using matrix:\n";
        print_matrix(m, log);

        for (std::size_t j{ 0U }; j != m.NUMBER_OF_COLS; ++j)
        {
            std::size_t i{ 0U };
            for (auto const& el : m.get_col(j))
            {
                if (m[i][j] != el)
                {
                    passed = false;
                    err << StreamColors::RED << "[ERROR] Range-iterator value and bracket square operator value not equal:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
                log << " [" << i << "][" << j << "]:" << el << " && " << m[i][j] << "\n";
                ++i;
            }
        }
        log << "Range based loop (column) end\n";


        m = Matrix<3U, 3U, int>{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

        log << "Construction from view begin\n";
        log << "Using matrix:\n";

        print_matrix(m, log);

        auto v{ m.get_col(0U) };
        auto const r0{ get_vector_row(v) };
        auto const c0{ get_vector_col(v) };

        check_matrix_and_range(log, err, passed, c0, v);
        check_matrix_and_range(log, err, passed, r0, v);

        v = m.get_row(1U);
        auto const r1{ get_vector_row(v) };
        auto const c1{ get_vector_col(v) };

        check_matrix_and_range(log, err, passed, c1, v);
        check_matrix_and_range(log, err, passed, r1, v);

        log << "Construction from view end\n";

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_math_operations_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Math operations" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        constexpr auto       auto_identity{ get_identity<3U, int>() };
        constexpr std::array manual_identity{ 1, 0, 0, 0, 1, 0, 0, 0, 1 };

        log << "Identity matrix:\n";
        print_matrix(auto_identity, log);

        check_matrix_and_range(log, err, passed, auto_identity, manual_identity);

        log << "Matrices comparing begin\n";

        constexpr Matrix<3U, 3U, int> m{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        log << "Matrix m:\n";
        print_matrix(m, log);

        if (m != m)
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrix is not equal to itself:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m == m\n";

        if (m == auto_identity)
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices are equal:\n" << StreamColors::RESET;
            print_matrix(m, log);
            print_matrix(auto_identity, log);
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m != identity\n";

        constexpr Matrix<3U, 2U> m1{ };
        constexpr Matrix<2U, 3U> m2{ };

        if constexpr (m1 == m2)
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices with different sizes are equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m1<3, 2> != m2<2, 3>\n";

        log << "Matrices comparing end\n";

        log << "Matrices transpose begin\n";

        log << "m:\n";
        print_matrix(m, log);

        constexpr auto m_t{ m.get_transponsed() };

        log << "mT:\n";
        print_matrix(m_t, log);

        for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
        {
            auto const v{ m.get_row(i) };
            auto const v_t{ m_t.get_col(i) };
            for (std::size_t j{ 0U }; j != v_t.NUMBER_OF_ELEMENTS; ++j)
            {
                if (v[j] != v_t[j])
                {
                    passed = false;
                    err << UnitTests::StreamColors::RED << "[ERROR] Wrong transposed matrix value:\n" << StreamColors::RESET;
                }
                else
                {
                    log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
                }
                log << " m[" << i << "][" << j << "] = " << v[j] << " && " << "m_t[" << j << "][" << i << "] = " << v_t[j] << "\n";
            }
        }

        constexpr Matrix<3U, 2U, int> m3{ 0, 1, 2, 3, 4, 5 };
        constexpr auto m3_t{ m3.get_transponsed() };
        constexpr std::array control3{ 0, 2, 4, 1, 3, 5 };

        log << "m:\n";
        print_matrix(m3, log);

        log << "mT:\n";
        print_matrix(m3_t, log);

        check_matrix_and_range(log, err, passed, m3_t, control3);

        constexpr Matrix<2U, 3U, int> m4{ 0, 1, 2, 3, 4, 5 };
        constexpr auto m4_t{ m4.get_transponsed() };
        constexpr std::array control4{ 0, 3, 1, 4, 2, 5 };

        log << "m:\n";
        print_matrix(m4, log);

        log << "mT:\n";
        print_matrix(m4_t, log);

        check_matrix_and_range(log, err, passed, m4_t, control4);

        log << "Matrices transpose end\n";

        constexpr auto m5{ m + m };
        log << "m:\n";
        print_matrix(m, log);

        log << "m + m:\n";
        print_matrix(m5, log);

        constexpr std::array check_sum{ 0 * 2, 1 * 2, 2 * 2, 3 * 2, 4 * 2, 5 * 2, 6 * 2, 7 * 2, 8 * 2 };
        check_matrix_and_range(log, err, passed, m5, check_sum);

        constexpr auto m6{ m - m };

        log << "m - m:\n";
        print_matrix(m6, log);

        constexpr std::array check_sub{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        check_matrix_and_range(log, err, passed, m6, check_sub);

        auto M{ m };
        log << "M = m\nM += m:\n";
        M += m;
        print_matrix(M, log);

        if (M != (m + m))
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m + m == m += m\n";

        M = m;
        log << "M = m\nM -= m:\n";
        M -= m;
        print_matrix(M, log);

        if (M != (m - m))
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m - m == m -= m\n";

        constexpr int  k { 3 };
        constexpr auto m7{ m * k };
        log << "m:\n";
        print_matrix(m, log);

        log << "m * " << k <<":\n";
        print_matrix(m7, log);

        constexpr std::array check_mul{ 0 * k, 1 * k, 2 * k, 3 * k, 4 * k, 5 * k, 6 * k, 7 * k, 8 * k };
        check_matrix_and_range(log, err, passed, m7, check_mul);

        M = m;
        log << "M = m\nM *= " << k << ":\n";
        M *= k;
        print_matrix(M, log);

        if (M != (k * m))
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m * " << k << " == m *= " << k << "\n";

        constexpr int  d{ 3 };
        const auto m8{ m / d };
        log << "m:\n";
        print_matrix(m, log);

        log << "m / " << d << ":\n";
        print_matrix(m8, log);

        constexpr std::array check_div{ 0 / d, 1 / d, 2 / d, 3 / d, 4 / d, 5 / d, 6 / d, 7 / d, 8 / d };
        check_matrix_and_range(log, err, passed, m8, check_div);

        M = m;
        log << "M = m\nM /= " << d << ":\n";
        M /= d;
        print_matrix(M, log);

        if (M != (m / d))
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrices are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m / " << d << " == m /= " << d << "\n";

        log << "Matrices multiplication begin:\n";

        log << "m:\n";
        print_matrix(m, log);
        if (m != m * get_identity<3U, int>())
        {
            passed = false;
            err << UnitTests::StreamColors::RED << "[ERROR] Matrix multiplicated on identity not equal to itself:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m * identity == m\n";

        {
            const Matrix<3U, 3U, int> M1{ 3, 4, -2, 1, 5, 8, -2, 4, 5 };
            const Matrix<3U, 3U, int> M2{ 6, 2, 1, 3, 9, -2, 2, 0, 2 };
        
            const auto M3{ M1 * M2 };
            const Matrix<3U, 3U, int> M3_control{ 26, 42, -9, 37, 47, 7, 10, 32, 0 };

            log << "m1:\n";
            print_matrix(M1, log);

            log << "m2:\n";
            print_matrix(M2, log);

            log << "m3:\n";
            print_matrix(M3, log);

            log << "m3_control:\n";
            print_matrix(M3_control, log);

            if (M3 != M3_control)
            {
                passed = false;
                err << UnitTests::StreamColors::RED << "[ERROR] Matrices multiplication wrong answer\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] m3 == m3_control\n" << StreamColors::RESET;
            }
        }
        {
            const Matrix<2U, 3U, int> M1{ 3, 4, -2, 1, 8, -2 };
            const Matrix<3U, 2U, int> M2{ 6, 3, 9, -2, 2, 2 };
            
            const auto M3{ M1 * M2 };
            const Matrix<2U, 2U, int> M3_control{ 50, -3, 74, -17 };

            log << "m1:\n";
            print_matrix(M1, log);

            log << "m2:\n";
            print_matrix(M2, log);

            log << "m3:\n";
            print_matrix(M3, log);

            log << "m3_control:\n";
            print_matrix(M3_control, log);

            if (M3 != M3_control)
            {
                passed = false;
                err << UnitTests::StreamColors::RED << "[ERROR] Matrices multiplication wrong answer\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] m3 == m3_control\n" << StreamColors::RESET;
            }
        }
        {
            const Matrix<1U, 3U, int> M1{ -2, 1, 8 };
            const Matrix<3U, 3U, int> M2{ -2, 3, 4, 6, 3, 9, -2, 0, 1 };

            const auto M3{ M1 * M2 };
            const Matrix<1U, 3U, int> M3_control{ -6, -3, 9 };

            log << "m1:\n";
            print_matrix(M1, log);

            log << "m2:\n";
            print_matrix(M2, log);

            log << "m3:\n";
            print_matrix(M3, log);

            log << "m3_control:\n";
            print_matrix(M3_control, log);

            if (M3 != M3_control)
            {
                passed = false;
                err << UnitTests::StreamColors::RED << "[ERROR] Matrices multiplication wrong answer\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] m3 == m3_control\n" << StreamColors::RESET;
            }
        }

        log << "Matrices multiplication end\n";

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_transformations_test(std::ostream& log, std::ostream& err)
    {
        static constexpr std::string_view TEST_NAME{ "Matrix transformations" };
        bool passed{ true };

        print_test_name(log, TEST_NAME);

        Matrix<3U, 3U, int> m
        {
            0, 1, 2, 
            3, 4, 5, 
            6, 7, 8 
        };
        constexpr int k{ 4 };
        constexpr std::array<int, 3U> control_1{ 3 + 6 * k, 4 + 7 * k, 5 + 8 * k };

        log << "m:\n";
        print_matrix(m, log);

        log << "Let sum (row 2) * " << k << " with row 1\n";
        m.sum_rows(2U, 1U, k);
        print_matrix(m, log);

        auto const row{ m.get_row(1U) };
        for (std::size_t i{ 0U }; i != m.NUMBER_OF_COLS; ++i)
        {
            if (row[i] != control_1[i])
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
            }
            log << row[i] << " == " << control_1[i] << "\n";
        }

        constexpr std::array<int, 3U> control_2{ 0 + 1 * -k, 27 + 32 * -k, 6 + 7 * -k };

        log << "Let sum (col 1) * " << -k << " with col 0\n";
        m.sum_cols(1U, 0U, -k);
        print_matrix(m, log);

        auto const col{ m.get_col(0U) };
        for (std::size_t i{ 0U }; i != m.NUMBER_OF_ROWS; ++i)
        {
            if (col[i] != control_2[i])
            {
                passed = false;
                err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
            }
            else
            {
                log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
            }
            log << col[i] << " == " << control_2[i] << "\n";
        }

        constexpr Matrix<3U> m1{ 2., 3., 1., 4., 5., 6., 1., 2., 2. };
        constexpr double d1{ -7. };
        constexpr Matrix<3U> m2{ -2., 1., 6., 2., 4., 2., 3., 6., 1. };
        constexpr double d2{ 20. };
        constexpr Matrix<3U> m3{ 22., 16., 43., 12., 15., 67., -33., 62., 10. };
        constexpr double d3{ -72107. };

        log << "Determinant calculation begin\n";

        print_matrix(m1, log);
        auto d{ get_determinant(m1) };
        if (!GameEngine::Geometry::Auxiliry::is_equal_with_precision(d, d1))
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << d << " == " << d1 << "\n";

        print_matrix(m2, log);
        d = get_determinant(m2);
        if (!GameEngine::Geometry::Auxiliry::is_equal_with_precision(d, d2))
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << d << " == " << d2 << "\n";

        print_matrix(m3, log);
        d = get_determinant(m3);
        if (!GameEngine::Geometry::Auxiliry::is_equal_with_precision(d, d3))
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << d << " == " << d3 << "\n";

        log << "Determinant calculation end\n";

        log << "Inverse matrix calculation begin\n";

        log << "m:\n";
        print_matrix(m1, log);
        auto const m1_inv{ get_inversed(m1) };
        constexpr Matrix<3U> control_inv_1{ 2. / 7., 4. / 7., -13. / 7., 2. / 7., -3. / 7., 8. / 7., -3. / 7., 1. / 7., 2. / 7. };
        if (m1_inv != control_inv_1)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
            print_matrix(control_inv_1, log, 16);
        }
        else
        {
            log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
        }
        log << "m_inv:\n";
        print_matrix(m1_inv, log, 16);

        log << "m:\n";
        print_matrix(m3, log);
        auto const m3_inv{ get_inversed(m3) };
        constexpr Matrix<3U> control_inv_3{ 572. / 10301., -358. / 10301., -61. / 10301., 333. / 10301., -1639. / 72107., 958. / 72107., -177. / 10301., 1892. / 72107., -138. / 72107. };
        if (m3_inv != control_inv_3)
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Control and calculated values are not equal:\n" << StreamColors::RESET;
            print_matrix(control_inv_3, log, 16);
        }
        else
        {
            log << StreamColors::GREEN << "[OK]" << StreamColors::RESET;
        }
        log << "m_inv:\n";
        print_matrix(m3_inv, log, 16);

        if (m1_inv * m1 != get_identity<3U>())
        {
            passed = false;
            err << StreamColors::RED << "[ERROR] Matrix multiplied to its inverse version is not equal to identity:\n" << StreamColors::RESET;
        }
        else
        {
            log << StreamColors::GREEN << "[OK] " << StreamColors::RESET;
        }
        log << "m_inv * m == identity\n";

        log << "Inverse matrix calculation end\n";


         m = Matrix<3U, 3U, int>
             {
                 0, 1, 2,
                 3, 4, 5,
                 6, 7, 8
             };

        std::size_t i{ 1U };
        log << "Remove column " << i << " from matrix:\n";
        print_matrix(m, log);

        {
            auto const _m{ remove_col(m, i) };
            constexpr std::array control{ 0, 2, 3, 5, 6, 8 };

            log << "Result:\n";
            print_matrix(_m, log);

            check_matrix_and_range(log, err, passed, _m, control);
        }

        i = 2U;
        log << "Remove row " << i << " from matrix:\n";
        print_matrix(m, log);

        {
            auto const _m{ remove_row(m, i) };
            constexpr std::array control{ 0, 1, 2, 3, 4, 5 };

            log << "Result:\n";
            print_matrix(_m, log);

            check_matrix_and_range(log, err, passed, _m, control);
        }

        i = 0U;
        std::size_t j{ 0U };
        log << "Remove row " << i << " and column " << j << " from matrix:\n";
        print_matrix(m, log);

        {
            auto const _m{ remove_row_and_col(m, i, j) };
            constexpr std::array control{ 4, 5, 7, 8 };

            log << "Result:\n";
            print_matrix(_m, log);

            check_matrix_and_range(log, err, passed, _m, control);
        }

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    " << TEST_NAME << "\n" << UnitTests::StreamColors::RESET;

        return passed;
    }
}