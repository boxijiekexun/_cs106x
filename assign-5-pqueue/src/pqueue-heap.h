#pragma once
#include "pqueue.h"
#include <string>
#include <iostream>
using namespace std;

class HeapPQueue : public PQueue {
public:
	HeapPQueue();
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    void mergeHelper(HeapPQueue* one,int index);
private:
    string* que;
    int caculation;
};
