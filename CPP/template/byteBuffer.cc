#include <vector>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>

class ByteBufferException : public std::exception {
  public:
    const char* what() const throw() { return "byte buffer exception"; }
};

template <typename T>
class ByteBuffer {
  public:
    ByteBuffer(size_t size = s_initSize);
    T read(size_t pos);
    void put(const T&, size_t pos);
  private:
    static const size_t s_initSize = 0x1000;
    std::vector<char> d_storage;
    size_t d_pos;
    size_t d_size;

};

template <typename T> inline
ByteBuffer<T>::ByteBuffer(size_t size) : 
    d_pos(0), d_size(size < s_initSize ? s_initSize : d_size)
{
    d_storage.reserve(d_size);
}

template <typename T> inline
T ByteBuffer<T>::read(size_t pos)
{
    T val = *(T*)&d_storage[pos];
    return val;
}

template <typename T> inline
void ByteBuffer<T>::put(const T& val, size_t pos)
{
    memcpy(&d_storage[pos], &val, sizeof(T));
}

struct Test {
    int a;
    double b;
    void print() const { std::cout << a + 5 << '\n'; } 
};

int main()
{
    ByteBuffer<Test> buffer;
    buffer.put(Test(), 0);
    Test t = buffer.read(0);
    t.print();
    return 0;
}
