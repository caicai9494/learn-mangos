#include <iostream>

// template deduce can be used to 
// detect type in compile time
template <class P, class T>
void fooImpl(P p, T t)
{
}

template <>
void fooImpl(int* ip, int i)
{
    std::cout << "deduce int\n";
}

// This technique is especially usefull where
// the function has to know the type info 
// from data member of 'ptr' object. 
// For example, T is an Iterator and Iterator::value_type 
// is needed 
template <class T>
void foo(T* ptr)
{
    fooImpl(ptr, *ptr);
}

int main()
{
    int i;
    foo(&i);

    return 0;
}
