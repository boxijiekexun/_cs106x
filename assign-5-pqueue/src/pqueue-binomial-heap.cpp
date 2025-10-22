#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {
}
BinomialHeapPQueue::~BinomialHeapPQueue() {
    // 1. 遍历 heap 中所有非空的根节点（每棵二项树的根）
    for (int i = 0; i < heap.size(); ++i) {
        if (heap[i] != nullptr) {
            // 2. 递归删除当前根节点及其所有子节点
            deleteSubtree(heap[i]);
            heap[i] = nullptr;  // 避免野指针（可选，但更严谨）
        }
    }
    // 3. 清空 heap 向量本身（若 Vector 类有 clear 方法，可补充）
    heap.clear();
}

void BinomialHeapPQueue::deleteSubtree(node* root) {
    if (root == nullptr) {
        return;  // base case：空节点直接返回
    }
    // 先递归删除所有子节点（子节点存储在 root->child 向量中）
    for (int i = 0; i < root->child.size(); ++i) {
        deleteSubtree(root->child[i]);
        root->child[i] = nullptr;  // 清空子节点指针（可选）
    }
    // 所有子节点删除后，再删除当前根节点
    delete root;
}



const node* BinomialHeapPQueue::peekHelper() const{
    node* minNode = nullptr;
    for(int i = 0;i < heap.size();i++){
        if(heap[i] != nullptr){
            if(minNode == nullptr || heap[i]->elem < minNode->elem){
                minNode = heap[i];
            }
        }
    }
    return minNode;
}

const string& BinomialHeapPQueue::peek() const {
    if(heap.isEmpty()){
        return kEmptyString;
    }
    else{
        const node* minNode = peekHelper();
        return minNode->elem;
    }
}

void BinomialHeapPQueue::mergeHelper(Vector<node*>& mainHeap,Vector<node*>& childHeap){
    // 有三种树：主堆树， 子堆树 ， 进位树
    node* carry = nullptr;
    int i = 0;

    // if(a)
    // a is nullptr return false
    // else return true

    while(i < childHeap.size() || i < mainHeap.size() || carry != nullptr){
        node* a = (i < mainHeap.size()) ? mainHeap[i] : nullptr;
        node* b = (i < childHeap.size()) ? childHeap[i] : nullptr;
        node* c = carry;

        // case 1: 只有一棵树 or 无树 ，直接放进去
        if((a && !b && !c) || (!a && b && !c) || (!a && !b && c)){
            node* temp = a ? a : (b ? b : c);
            if(i < mainHeap.size()){
                mainHeap[i] = temp;
            }
            else{
                mainHeap.add(temp);
            }
            carry = nullptr;
        }

        // case 2: 有两棵树, 产生进位
        else if((a && b && !c) || (a && !b && c) || (!a && b && c)){
            node* temp = nullptr;
            if(a && b){
                temp = link(a,b);
            }
            else if(a && c){
                temp = link(a,c);
            }
            else{
                temp = link(b,c);
            }
            carry = temp;
            if(i < mainHeap.size()){
                mainHeap[i] = nullptr;
            }
        }

        // case 3: 三棵树都存在，先合并两棵树，剩下的放进主堆，产生进位
        else if(a && b && c){
            string a1 = a->elem;
            string a2 = b->elem;
            string a3 = c->elem;
            node* temp1 = nullptr;
            node* temp2 = nullptr;
            if(a1 <= a2 && a1 <= a3){
                temp1 = link(b,c);
                temp2 = a;
            }
            else if(a2 <= a1 && a2 <= a3){
                temp1 = link(a,c);
                temp2 = b;
            }
            else{
                temp1 = link(a,b);
                temp2 = c;
            }
            if(i < mainHeap.size()){
                mainHeap[i] = temp2;
            }
            else{
                mainHeap.add(temp2);
            }

            carry = temp1;
        }
        else {
            // 无树的情况（理论上不会触发，保险处理）
            if (i < mainHeap.size()) {
                mainHeap[i] = nullptr;
            }
            carry = nullptr;
        }

        i++;
    }
}

string BinomialHeapPQueue::extractMin() {
    node* minNode = nullptr;
    int minIndex = -1;
    for(int i = 0;i < heap.size();i++){
        if(heap[i] != nullptr){
            if(minNode == nullptr || heap[i]->elem < minNode->elem){
                minNode = heap[i];
                minIndex = i;
            }
        }
    }
    if(minNode == nullptr){
        return kEmptyString;
    }
    string minNum = minNode->elem;
    heap[minIndex] = nullptr;
    Vector<node*> temp = minNode->child;
    //cout << minNum << "     ";
    delete minNode;
    mergeHelper(heap,temp);
    logSize--;
    return minNum;
}


node* BinomialHeapPQueue::link(node* t1,node* t2){
    if(t1->elem > t2->elem){
        swap(t1,t2);
    }
    t1->child.add(t2);
    t1->degree = t1->degree + 1;
    return t1;
}

void BinomialHeapPQueue::enqueue(const string& elem) {
    node* newTree = new node(elem);
    int i = 0;
    while(i < heap.size() && heap[i] != nullptr){
        newTree = link(newTree,heap[i]);
        heap[i] = nullptr;
        i++;
    }
    // 扩容
    if(i < heap.size()){
        heap[i] = newTree;
    }
    else{
        heap.add(newTree);
    }
    logSize++;
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one, BinomialHeapPQueue * two) {
    BinomialHeapPQueue* que = new BinomialHeapPQueue();
    que->heap = one->heap;
    que->mergeHelper(que->heap,two->heap);
    que->logSize = one->logSize + two->logSize;
    return que;
}
