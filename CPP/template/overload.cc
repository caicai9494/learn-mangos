#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>

template <typename T>
const T& max(const T& a, const T& b)
    // pass by const-ref
{
    return a > b ? a : b;
}

inline
const char* max(const char* a, const char* b)
    // pass by value
{
    return ::strcmp(a, b) < 0 ? b : a;
}

template <typename T>
const T& max(const T& a, const T& b, const T& c)
    // pass by const-ref
{
    return max(max(a, b), c);
}

int main()
{
    int a = max(1,2,3);
    std::cout << a << '\n';

    const char* p1 = "a";
    const char* p2 = "b";
    const char* p3 = "c";

    const char* b = max(p1,p2,p3); 
    // an warning since p1, p2, p3 are passed by value
    // and a reference gets returned
    std::cout << b << '\n';
    return EXIT_SUCCESS;
}
