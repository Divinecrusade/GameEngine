#pragma once

#include <Matrix.hpp>

#include <iostream>
#include <iomanip>
#include <string_view>
#include <type_traits>
#include <vector>


namespace UnitTests
{
    using namespace GameEngine::Geometry;

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
        static_assert(arithmetic_like<DummyExceptArithmetic>);

        std::ostream& operator<<(std::ostream& out, DummyExceptArithmetic const&)
        {
            out << "Dummy";
            return out;
        }


        static void print_test_name(std::ostream& log, std::string_view name)
        {
            log << SEPRATOR << "\n";
            log << "|" << std::setfill('-') << std::left << std::setw(SEPRATOR.length() - 2) << name << std::setfill(' ') << "|\n";
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
        static void print_matrix(Matrix<M, N, T> const& m, std::ostream& out)
        {
            for (std::size_t i{ 0U }; i != M; ++i)
            {
                for (std::size_t j{ 0U }; j != N; ++j)
                {
                    out << std::setw(8) << m[i][j];
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

    static bool is_pass_default_constructor_test(std::ostream& log, std::ostream& err)
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

    static bool is_pass_copy_constructor_test(std::ostream& log, std::ostream& err)
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

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix parameter constructor\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix parameter constructor\n" << UnitTests::StreamColors::RESET;

        return passed;
    }

    static bool is_pass_swapping_test(std::ostream& log, std::ostream& err)
    {
        bool passed{ true };

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

        if (passed) log << UnitTests::StreamColors::GREEN << "[SUCCESS] Matrix columns and rows swapping\n" << UnitTests::StreamColors::RESET;
        else        err << UnitTests::StreamColors::RED   << "[FAIL]    Matrix columns and rows swapping\n" << UnitTests::StreamColors::RESET;

        return passed;
    }
}