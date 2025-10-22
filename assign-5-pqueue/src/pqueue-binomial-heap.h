#pragma once
#include "pqueue.h"
#include <string>
#include <iostream>
#include "vector.h"
using namespace std;

struct node{
    string elem;
    int degree = -1;  //node作为的阶数

    Vector<node*> child;
    node(){}
    node(string s):elem(s),degree(0){}
};

class BinomialHeapPQueue : public PQueue {
private:
    Vector<node*> heap;


public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    void mergeHelper(Vector<node*>& mainHeap,Vector<node *>& childHeap);
    node* link(node* t1,node* t2);
    const node* peekHelper() const;
    void deleteSubtree(node* root);
};

