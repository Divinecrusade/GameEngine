#include <iostream>


int main()
{
    return []() noexcept { std::puts("Matrices project test\n"); return EXIT_SUCCESS; }();
}