#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {
    //cout << "struct " << endl;
}
VectorPQueue::~VectorPQueue() {}

const string& VectorPQueue::peek() const {
    //cout << "oh here" << endl;
    if(que.isEmpty()){
        return kEmptyString;
    }
    else{
        return que[0];
    }
}

string VectorPQueue::extractMin() {
    string s;
    if(!que.isEmpty()){
        s = que[0];
        que.remove(0);
    }
    logSize--;
    return s;
}

void VectorPQueue::enqueue(const string& elem) {
    if(que.isEmpty()){
        que.add(elem);
    }
    else{
        for(int i = 0;i < que.size();i++){
            if(i == 0 && elem <= que[i]){
                que.insert(0,elem);
                break;
            }
            else if(i > 0 && elem >= que[i - 1] && elem <= que[i]){
                que.insert(i,elem);
                break;
            }
            else if(i == que.size() - 1 && que[i] <= elem){
                que.insert(i + 1,elem);
                break;
            }
        }
    }
    logSize++;
}

VectorPQueue *VectorPQueue::merge(VectorPQueue* one, VectorPQueue* two ) {
    VectorPQueue* Newque = new VectorPQueue();
    Vector<string> temp;
    for(const string &s: one->que){
        temp.add(s);
    }
    for(const string &s: two->que){
        temp.add(s);
    }
    cout << "one and two size is " << one->size() << " " << two->size() << endl;
    cout << Newque->que.size() << endl;
    cout << temp.size() << endl;
    for(const string &s: temp){
        Newque->enqueue(s);
    }
    cout << Newque->que.size() << endl;
    return Newque;
}
