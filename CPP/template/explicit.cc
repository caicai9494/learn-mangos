#include <iostream>

template <typename T>
class Stack {
  public:
    void push(const T&) {}
    void pop() {}
    //T top() { return T(); }
    T top() {}
};

template int Stack<int>::top();

int main()
{
    return 0;
}
