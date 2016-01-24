#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

void func()
{
    std::cout << "function pointer - thread 1\n";
}

int main()
{
    // initialize a thread from a function
    std::thread th1(&func);

    // initialize a thread from a lambda
    std::thread th2([](){
	std::cout << "anonymous function - thread 2\n";
    });
    std::cout << "main thread\n";

    th1.join();
    th2.join();

    // a simple implementation of barrier
    std::vector<std::thread> workers;
    for (int i = 0; i < 10; ++i) {
	// non-deterministic
	workers.push_back(std::thread([i](){
	    std::cout << "hi from thread " << i << " \n";
	}));
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread& th){
	th.join();
    });

    return EXIT_SUCCESS;
}
