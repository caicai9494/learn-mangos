#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cassert>

#include <unistd.h>

void tFunc(int& i)
{
    std::cout << "hi from thread " << i << " \n";
}

void test(std::vector<std::thread>& workers)
{
    for (int i = 0; i < 10; ++i) {
	// non-deterministic
        auto t = std::thread(tFunc, std::ref(i));
	// type checking inside the constructor
	// dangerous to pass in a ref
	// now you need to worry about the lifetime of i
	workers.push_back(std::move(t));
	assert(!t.joinable());
    }
}

int main()
{
    // move semantics
    // uniqueness and aliasing
    // a simple implementation of barrier
    std::vector<std::thread> workers;
    for (int i = 0; i < 10; ++i) {
	// non-deterministic
        auto t = std::thread(tFunc, std::ref(i));
	// type checking inside the constructor
	workers.push_back(std::move(t));
	assert(!t.joinable());
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread& th){
	assert(th.joinable());
	th.join();
    });

    workers.clear();
    test(workers);
    std::for_each(workers.begin(), workers.end(), [](std::thread& th){
	assert(th.joinable());
	th.join();
    });

    return EXIT_SUCCESS;
}
