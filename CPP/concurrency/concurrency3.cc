#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cassert>

#include <unistd.h>

struct List {
    List() : d_count(0), d_node(nullptr) {}

    struct Node {
	int value;
	Node* next;
	Node(int v) : value(v), next(nullptr) {}
    };

    void insert(int v) 
    {
	Node* node = new Node(v);
	node->next = d_node;
	d_node = node;
    }

    int count() const 
    {
	int count = 0;
	Node* cur = d_node;
	while (nullptr != cur) {
	    ++count;
	    cur = cur->next;
	}
	return count;
    }

    int d_count;
    Node* d_node;

}; 

void tFunc(List& list)
{
    for (int i = 0; i < 100; ++i) {
	list.insert(i);
    }
}

int main()
{
    List list;

    std::vector<std::thread> workers;
    for (int i = 0; i < 10; ++i) {
        auto t = std::thread(tFunc, std::ref(list));
	// racing 
	workers.push_back(std::move(t));
	assert(!t.joinable());
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread& th){
	assert(th.joinable());
	th.join();
    });

    return list.count();
}
