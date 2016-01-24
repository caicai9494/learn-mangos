#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>
#include <string>
#include <iterator>
#include <mutex>
#include <boost/filesystem.hpp>

namespace BF = boost::filesystem;

typedef std::vector<std::string> string_vector;
typedef std::vector<BF::path> path_vector;

const int THREAD_MAX = 16;

// we could consider to start the tasks in batch
struct FileDir {
    string_vector files;

    typedef string_vector::iterator IT; 

    std::vector<BF::path> paths;

    FileDir() = default; 

    FileDir(FileDir&& rhs) 
	: files(std::move(rhs.files)), paths(std::move(rhs.paths)) 
    {}

    FileDir& operator= (FileDir&& rhs) 
    {
	files = std::move(rhs.files);
	paths = std::move(rhs.paths);
	return *this;
    }
};

// monitor pattern
class MonitorFileDir {

  private:

    FileDir d_filedir;
    std::mutex d_mutex;

  public:
    
    FileDir::IT begin() 
    {
	std::lock_guard<std::mutex> lck(d_mutex);
	return d_filedir.files.begin();
    }

    FileDir::IT end() 
    {
	std::lock_guard<std::mutex> lck(d_mutex);
	return d_filedir.files.end();
    }

    bool isDirEmpty() 
    {
	std::lock_guard<std::mutex> lck(d_mutex);
	return d_filedir.paths.empty();
    }

    void putFile(const std::string& file) 
    {
	// in reality, it's unsafe to call lock directly
	//  because if what's in between throw, the thread
	//  will remain blocked forever
	// Use lock_guard, which is a raii object
	std::lock_guard<std::mutex> lck(d_mutex);
	//d_mutex.lock();
	d_filedir.files.push_back(file);
	//d_mutex.unlock();
    }
    void putDir(const BF::path& pth) 
    {
	std::lock_guard<std::mutex> lck(d_mutex);
	d_filedir.paths.push_back(pth);
    }

    path_vector getDirs(int n)
    {
	path_vector dirs;
	std::lock_guard<std::mutex> lck(d_mutex);
	for (int i = 0; i < n && !d_filedir.paths.empty(); ++i) {
	    dirs.push_back(std::move(d_filedir.paths.back()));
	    d_filedir.paths.pop_back();
	}
	return dirs;
    }
};

// Use shared memory
void listDirectory2(BF::path&& dir, MonitorFileDir& ret) 
{
    for (BF::directory_iterator it(dir); it != BF::directory_iterator(); ++it) {
	if (BF::is_directory(it->path())) {
	    ret.putDir(it->path());
	} else {
	    ret.putFile(dir.string());
	}
    }
}

void test2_shared(MonitorFileDir& fd)
{
    string_vector files;

    // run in one thread
    //  chance for optimization
    while (!fd.isDirEmpty()) {

	// run in multiple threads
	path_vector dirTodo = fd.getDirs(THREAD_MAX);
	std::vector<std::future<void>> ftrs;
	while (!dirTodo.empty()) {

	    auto ftr = std::async(std::launch::async, &listDirectory2, 
		                  std::move(dirTodo.back()), std::ref(fd));
	    dirTodo.pop_back();

	    ftrs.push_back(std::move(ftr));
	}

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

    std::copy(fd.begin(), fd.end(), 
	      std::ostream_iterator<std::string>(std::cout, "\n >"));
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
	std::cout << "usage: .tsk path\n";
	return EXIT_FAILURE;
    } 
    std::string root = argv[1]; 
    MonitorFileDir fd;
    fd.putDir(root);

    // the following code is problemtic
    // heap may be corrupted in some cases
    // because we did some clever optimization 
    // and it turns out it is not that clever
    auto fut1 = std::async(std::launch::async, &test2_shared, std::ref(fd));
    auto fut2 = std::async(std::launch::async, &test2_shared, std::ref(fd));
    fut1.wait();
    fut2.wait();

    return EXIT_SUCCESS;
}

