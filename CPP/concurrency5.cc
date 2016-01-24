#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>
#include <string>

int main()
{
    std::cout << "main thread id: " << std::this_thread::get_id() << '\n';

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 20; ++i) {
	//auto ftr = std::async([]{
	auto ftr = std::async(std::launch::async, []{
	    std::this_thread::sleep_for(std::chrono::seconds(1));
	    std::cout << std::this_thread::get_id() << '\n';
	});
	futures.push_back(std::move(ftr));
    }

    // barrier
    std::for_each(futures.begin(), futures.end(), [](std::future<void>& fut) 
    {
	fut.wait();
    });

    std::this_thread::sleep_for(std::chrono::seconds(40));
    return EXIT_SUCCESS;
}
