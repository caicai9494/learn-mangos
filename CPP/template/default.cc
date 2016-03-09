#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>

#ifdef CANNOTCOMPILE
template <double VAL> // 'double' cannot be nontype parameter
void foo() 
{
}

template <std::string STR>  // 'class' object cannot be nontype parameter 
void foo() 
{}

#endif

template <const char* STR>  // does work in c++11
void foo() 
{}


int main()
{
    return EXIT_SUCCESS;
}
