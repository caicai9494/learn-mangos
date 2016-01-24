#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <exception>

std::string normalfun()
{
    throw std::runtime_error("throw from normal function");
    return "normal function\n";
}

void fun(std::promise<std::string>& prms)
{
    try {
	throw std::runtime_error("throw from future");
	prms.set_value("yoyo");
    } catch (...) {
	// exception not handled in thread
	// so it's ok to catch all
	prms.set_exception(std::current_exception());
    }
	
}

int main()
{
    std::promise<std::string> prms;
    std::future<std::string> ftr = prms.get_future();
    // create a shared state
    std::thread th(&fun, std::ref(prms));
    // promise : set the value / exception
    // future : get the value / exception


    try {

	std::string ret = ftr.get();
	std::cout << ret << '\n';

    } catch (std::exception& ex) {

	std::cout << ex.what() << '\n';

    }

    // std::string ret = ftr.get();
    // this will cause an error because
    // ftr.get() will move so the second time
    // this is called, the shared state doesn't exist

    th.join();

    // there's an easier way : async
    //  and normalfun just looks like a normal function
    auto ftr2 = std::async(&normalfun);
    // if fun throws, it will be rethrown in future::get()
    //  dtor of ftr2 will force the thread to be finished
    try {

	std::cout << ftr2.get() << '\n';

    } catch (std::exception& ex) {

	std::cout << ex.what() << '\n';

    }

    return EXIT_SUCCESS;
}
