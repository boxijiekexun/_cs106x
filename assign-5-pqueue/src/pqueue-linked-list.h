#pragma once
#include "pqueue.h"
#include <iostream>
#include <string>

using namespace std;

struct ListNode{
    string word;
    ListNode* pre;
    ListNode* next;

    ListNode():word(""),pre(nullptr),next(nullptr){}
    ListNode(string words): word(words),pre(nullptr),next(nullptr){}
    ListNode(string words,ListNode* pre,ListNode* next): word(words),pre(pre),next(next){}
};

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    ListNode* list;
    int size;
};
