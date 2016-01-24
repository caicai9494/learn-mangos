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
	return d_filedir.files.begin();
    }

    FileDir::IT end() 
    {
	return d_filedir.files.end();
    }

    bool isDirEmpty() 
    {
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

// Use move semantic
FileDir listDirectory(BF::path&& dir) 
{
    FileDir ret;

    for (BF::directory_iterator it(dir); it != BF::directory_iterator(); ++it) {
	if (BF::is_directory(it->path())) {
	    ret.paths.push_back(it->path());
	} else {
	    ret.files.push_back(dir.string());
	}
    }

    return ret;
}

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

void test1_move(const std::string& root)
{
    path_vector dirTodo;
    dirTodo.push_back(root);

    string_vector files;

    // in this code, all data is moved between threads, rather than copying
    //  thus we avoid sync problems and is less error-prone
    while (!dirTodo.empty()) {

	std::vector<std::future<FileDir>> ftrs;

	// starting tasks
	//  here, we are only gonna start up to 'THREAD_MAX' threads
	for (int i = 0; i < THREAD_MAX && !dirTodo.empty(); ++i) {

	    auto ftr = std::async(std::launch::async, &listDirectory, std::move(dirTodo.back()));
	    dirTodo.pop_back();

	    ftrs.push_back(std::move(ftr));
	}

	try {

	    // barrier
	    while (!ftrs.empty()) {

	        auto ftr = std::move(ftrs.back());
	        ftrs.pop_back();

		FileDir fd = ftr.get();
		std::move(fd.files.begin(), fd.files.end(), std::back_inserter(files));
		std::move(fd.paths.begin(), fd.paths.end(), std::back_inserter(dirTodo));
	   }

	} catch (std::exception& ex) {
	    std::cout << ex.what() << '\n';
	}
    }

    std::copy(files.begin(), files.end(), 
	      std::ostream_iterator<std::string>(std::cout, "\n >"));
}

void test2_shared(const std::string& root)
{
    MonitorFileDir fd;
    fd.putDir(root);

    string_vector files;

    // in this code, all data is moved between threads, rather than copying
    //  thus we avoid sync problems and is less error-prone
    while (!fd.isDirEmpty()) {

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

    auto startTimer = std::chrono::system_clock::now();
    //test1_move(root);
    test2_shared(root);
    auto endTimer = std::chrono::system_clock::now();

    auto dur = endTimer - startTimer;
    auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    std::cout << "test1 move durMs: " << durMs.count() << '\n';

    /*
    auto startTimer2 = std::chrono::system_clock::now();
    test2_shared(root);
    auto endTimer2 = std::chrono::system_clock::now();

    auto dur2 = endTimer2 - startTimer2;
    auto durMs2 = std::chrono::duration_cast<std::chrono::milliseconds>(dur2);
    std::cout << "test2 shared durMs: " << durMs2.count() << '\n';
    */

    // unbounded number of threads
    // not desirable

//    auto fut = async(std::launch::async, &listDirectory, std::move(root));
//
//    try {
//	string_vector listing = fut.get();
//	std::for_each(listing.begin(), listing.end(), [](std::string& s) {
//	    std::cout << s << " ";
//	});
//    } catch (std::exception& e) {
//	std::cout << e.what() << '\n';
//    } catch (...) {
//	std::cout << "unexpected exception\n";
//    }

    return EXIT_SUCCESS;
}

