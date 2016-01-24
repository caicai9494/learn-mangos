#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>
#include <string>
#include <iterator>
#include <boost/filesystem.hpp>

namespace BF = boost::filesystem;

typedef std::vector<std::string> string_vector;
const int THREAD_MAX = 16;

string_vector listDirectory(std::string&& dir) 
    // the number of threads is unbounded
    //  so may not be a good idea
{
    string_vector listing;
    std::string dirStr("\n> ");
    dirStr += dir;
    dirStr += ":\n ";
    listing.push_back(dirStr);

    std::vector<std::future<string_vector>> futs;
    for (BF::directory_iterator it(dir); it != BF::directory_iterator(); ++it) {
	if (BF::is_directory(it->path())) {
	    auto fut = async(std::launch::async, &listDirectory, std::move(it->path().string()));
	    futs.push_back(std::move(fut));
	} else {
	    listing.push_back(it->path().string());
	}
    }

    std::for_each(futs.begin(), futs.end(), [&listing](std::future<string_vector>& fut) {
	string_vector lst = fut.get();
	std::copy(lst.begin(), lst.end(), std::back_inserter(listing));
    });

    return listing;
}

// we could consider to start the tasks in batch
struct FileDir {
    string_vector files;
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

typedef std::vector<BF::path> path_vector;

FileDir listDirectory2(BF::path&& dir) 
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

int main(int argc, char* argv[])
{
    if (argc != 2) {
	std::cout << "usage: .tsk path\n";
	return EXIT_FAILURE;
    }
    std::string root = argv[1]; 

    path_vector dirTodo;
    dirTodo.push_back(root);

    string_vector files;

    while (!dirTodo.empty()) {

	std::vector<std::future<FileDir>> ftrs;

	// starting tasks
	//  here, we are only gonna start up to 'THREAD_MAX' threads
	for (int i = 0; i < THREAD_MAX && !dirTodo.empty(); ++i) {

	    auto ftr = std::async(std::launch::async, &listDirectory2, std::move(dirTodo.back()));
	    dirTodo.pop_back();

	    ftrs.push_back(std::move(ftr));
	}

	try {

	    // barrier
	    while (!ftrs.empty()) {

	        auto ftr = std::move(ftrs.back());
	        ftrs.pop_back();

		FileDir fd = ftr.get();
		std::copy(fd.files.begin(), fd.files.end(), std::back_inserter(files));
		std::copy(fd.paths.begin(), fd.paths.end(), std::back_inserter(dirTodo));
	   }

	} catch (std::exception& ex) {
	    std::cout << ex.what() << '\n';
	    //ftrs.pop_back();
	}
    }

    std::copy(files.begin(), files.end(), 
	      std::ostream_iterator<std::string>(std::cout, "\n >"));


    
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

