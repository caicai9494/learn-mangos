#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>
#include <string>
#include <iterator>
#include <mutex>
#include <deque>
#include <condition_variable>

#include <boost/filesystem.hpp>

namespace BF = boost::filesystem;
const int THREAD_MAX = 16;

template<class T>
class MsgQueue {

  private:

    std::deque<T> d_queue;
    std::condition_variable d_cond;
    std::mutex d_mutex;

  public:
    void send(T&& msg)
    {
	{
	    std::lock_guard<std::mutex> lck(d_mutex);
	    d_queue.push_front(std::move(msg));
	}
	d_cond.notify_one();
    }

    T receive()
    {
	std::unique_lock<std::mutex> lck(d_mutex);
	d_cond.wait(lck, [this] { return !d_queue.empty();});
	T msg = std::move(d_queue.back());
	return msg;
    }

};

// Use condition variable
// Server pattern:
// waits for a job. Once it arrives,
//  perform computation
void listDirServer(MsgQueue<BF::path>& dirQueue,
                   MsgQueue<std::string>& fileQueue) 
{
    for (;;) {
	BF::path dir = dirQueue.receive();

	for (BF::directory_iterator it(dir); it != BF::directory_iterator(); ++it) {

	    BF::path pth = std::move(it->path());
	    if (BF::is_directory(pth)) {
		dirQueue.send(std::move(pth));
	    } else {
		std::string file = std::move(pth.string());
		fileQueue.send(std::move(file));
	    }
	}

    }
}

void printDirServer(MsgQueue<std::string>& fileQueue) 
{
    for (;;) {
	std::string file = fileQueue.receive();
	std::cout << "> " << file << '\n';
    }
}

void test3_cv(BF::path&& root)
{
    MsgQueue<BF::path> dirQueue; 
    MsgQueue<std::string> fileQueue; 

    dirQueue.send(std::move(root));

    std::vector<std::future<void>> ftrs;
    for (int i = 0; i < THREAD_MAX; ++i) {

	auto ftr = std::async(std::launch::async, &listDirServer, std::ref(dirQueue), std::ref(fileQueue));

	ftrs.push_back(std::move(ftr));
    }
    auto ftr = std::async(std::launch::async, &printDirServer, std::ref(fileQueue));
    ftrs.push_back(std::move(ftr));


    try {

	// barrier
	while (!ftrs.empty()) {

	    auto ftr = std::move(ftrs.back());
	    ftrs.pop_back();
	    // use wait if we don't care about the return value of future
	    ftr.wait();
       }

    } catch (std::exception& ex) {
	std::cout << ex.what() << '\n';
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
	std::cout << "usage: .tsk path\n";
	return EXIT_FAILURE;
    } 
    std::string root = argv[1]; 
    BF::path pth(root);

    test3_cv(std::move(pth));

    return EXIT_SUCCESS;
}

