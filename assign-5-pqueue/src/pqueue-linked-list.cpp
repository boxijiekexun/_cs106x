#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    list = nullptr;
    size = logSize;
}
LinkedListPQueue::~LinkedListPQueue() {
    ListNode* cur = list;
    while(cur != nullptr){
        ListNode* trash = cur;
        cur = cur->next;
        delete trash;
    }
    list = nullptr;  // 避免野指针
    size = 0;
}

const string& LinkedListPQueue::peek() const {
    if(list == nullptr){
        return kEmptyString;
    }
    else{
        return list->word;
    }
}

void helper(ListNode* list){
    ListNode* cur = list;
    int count = 0;
    while(cur != nullptr){
        //cout << cur->word << " ";
        cur = cur->next;
        count++;
    }
    cout << count << endl;
}

string LinkedListPQueue::extractMin() {
    //cout << "extractMin" << endl;
    //helper(list);
    string s = "";
    if(list != nullptr){
        ListNode* cur = list;
        s = cur->word;
        list = list->next;
        if (list != nullptr) {  // 新增判断：若新头指针非空，再修改pre
            list->pre = nullptr;
        }
        delete cur;
        logSize--;
    }
    // cout << s << " ";
    return s;
}

void LinkedListPQueue::enqueue(const string& elem) {
    if(list == nullptr){
        cout << "sss" << endl;
        ListNode* node = new ListNode(elem);
        list = node;
    }
    else{
        //cout << "s" << endl;
        ListNode* node = new ListNode(elem);
        ListNode* cur = list;
        if(elem <= cur->word){
            node->next = list;
            list->pre = node;
            node->pre = nullptr;
            list = node;
        }
        else{
            while(cur->next != nullptr && cur->next->word < elem){
                cur = cur->next;
            }
            if(cur->next == nullptr){
                cur->next = node;
                node->pre = cur;
            }
            else{
                node->next = cur->next;
                cur->next->pre = node;
                cur->next = node;
                node->pre = cur;
            }
        }
    }
    logSize++;
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue *one, LinkedListPQueue * two) {
    ListNode* cur = two->list;
    while(cur != nullptr){
        string s = cur->word;
        one->enqueue(s);
        cur = cur->next;
    }
    return one;
}


