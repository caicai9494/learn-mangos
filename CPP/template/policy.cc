#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

template<typename T>
struct OpNewCreator {
    static T* create()
    {
	return new T();
    }
}; 

template<typename T>
struct MallocCreator {
    static T* create()
    {
	void* buffer = ::malloc(sizeof(T));
	if (!buffer) {
	    return nullptr;
	}
	return new(buffer)(T);;
    }
}; 

template<typename CreatorPolicy>
class IntManager : public OpNewOperator<CreatorPolicy>
{
}

int main()
{
    return EXIT_SUCCESS;
}
