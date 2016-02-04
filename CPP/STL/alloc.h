#ifndef ALLOC_H
#define ALLOC_H

#include <cstddef>
#include <iostream>
#include <new>

namespace LC {

template <class T>
class Alloc {
  public:

    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    T* allocate(difference_type size, pointer t);
    void deallocate(pointer p);
    void construct(pointer p, const_reference value);
    void destroy(pointer p);
};


template <class T> inline 
T* Alloc<T>::allocate(difference_type size, pointer t)
{
    return NULL;
}

template <class T> inline 
void Alloc<T>::deallocate(pointer p)
{
}

template <class T> inline 
void Alloc<T>::construct(pointer p, const_reference value) {}

template <class T> inline 
void Alloc<T>::destroy(pointer p) {}


}  // close namespace LC

#endif
