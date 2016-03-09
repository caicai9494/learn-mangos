#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>

template <typename T>
class Base {
  public:
    ~Base() {}
    virtual void exit() {}
};

template <typename T>
class Derive : public Base<T> {
  public:
    void foo() 
    {
	this->exit();
	//exit(); //this would refer to (void (*)(int status))
    }
};

int main()
{
    // useful to ensure all variable has default value
    // in template function
    std::cout << int() << '\n';
    std::cout << float() << '\n';
    std::cout << double() << '\n';
    return EXIT_SUCCESS;
}
