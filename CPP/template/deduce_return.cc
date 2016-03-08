#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

template <typename RET, typename T>
const RET& max(const T& a, const T& b)
{
    return a > b ? a : b;
}

int main()
{
    // specify the return type
    // but let compiler deduce the parameter
    int x = max<int>(5, 4);
    std::cout << x << '\n';

    double x2 = max<double>(5.9, 4.0);
    std::cout << x2 << '\n';

    char x3 = max<char>('a', 'b');
    std::cout << x3 << '\n';

    return EXIT_SUCCESS;
}
