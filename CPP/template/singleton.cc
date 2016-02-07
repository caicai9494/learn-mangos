#include <iostream>

template <typename T>
class Singleton {
  public:

    // This code is concurrently safe in c++1x.
    // A thread will wait if the static variable 
    // is being initialized.
    static T* instance()
    {
	// Using the default constructor
	static T instn;
	return &instn;
    }

  protected:

    Singleton() = default;
};

struct Test {
    void foo() 
    {
	std::cout << "test foo\n";
    }
};

// This macro is handy
#define s_test Singleton<Test>::instance()

int main()
{
    s_test->foo();
    return 0;
}
