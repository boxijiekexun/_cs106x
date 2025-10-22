#include "pqueue-heap.h"
using namespace std;

HeapPQueue::HeapPQueue() {
    caculation = 4;
    que = new string[caculation + 1]();
    logSize = 0;
}
HeapPQueue::~HeapPQueue() {
    delete[] que;
}

const string& HeapPQueue::peek() const {
    if(que[1] == ""){
        return kEmptyString;
    }
    else{
        return que[1];
    }
}

string HeapPQueue::extractMin() {
    string s = que[1];
    que[1] = que[logSize];
    logSize--;
    int index = 1;
    while(index != logSize){
        if(index * 2 <= logSize && index * 2 + 1 <= logSize){
            if(que[index] <= que[index * 2] && que[index * 2 + 1] >= que[index]){
                break;
            }
            else if(que[index * 2] <= que[index * 2 + 1]){
                //cout << que[index] << " " << que[index * 2] << " " << que[index * 2 + 1] << endl;
                swap(que[index],que[index * 2]);
                index *= 2;
            }
            else{
                //cout << que[index] << " " << que[index * 2] << " " << que[index * 2 + 1] << endl;
                swap(que[index],que[index * 2 + 1]);
                index = 2 * index + 1;//
            }
            continue;
        }
        else if(index * 2 <= logSize && index * 2 + 1 > logSize){
            //cout << que[index] << " " << que[index * 2] << endl;
            if(que[index] <= que[index * 2]){
                break;
            }
            else{
                swap(que[index],que[index * 2]);
                index *= 2;
            }
        }
        else{
            break;
        }
    }
    return s;
}

void HeapPQueue::enqueue(const string& elem) {
    //cout << elem << endl;
    if(logSize == caculation){
        string* temp = new string[2 * caculation + 1];
        caculation *= 2;
        for(int i = 1;i <= logSize;i++){
            temp[i] = que[i];
        }
        que = temp;
    }
    que[++logSize] = elem;
    int index = logSize;
    while(index != 1){
        if(que[index] <= que[index / 2]){
            swap(que[index],que[index / 2]);
            index /= 2;
        }
        else{
            break;
        }
    }
}


HeapPQueue *HeapPQueue::merge(HeapPQueue* one, HeapPQueue *two) {
    cout << one->logSize << endl;
    cout << two->logSize << endl;
    cout << "que 0 is " << two->que[0] << endl;;
    cout << two->que[two->logSize] << " ";
    for(int i = 1;i <= two->logSize;i++){
        if(one->caculation == one->logSize){
            string* temp = new string[2 * one->caculation + 1];
            one->caculation *= 2;
            for(int i = 1;i <= one->logSize;i++){
                temp[i] = one->que[i];
            }
            one->que = temp;
        }
        one->logSize++;
        one->que[one->logSize] = two->que[i];
    }
    int length = one->logSize;
    int index = length / 2;
    for(int i = index;i >= 1;i--){
        one->mergeHelper(one,i);
    }
    return one;
}

void HeapPQueue::mergeHelper(HeapPQueue* one,int index){
    int length = one->logSize;
    while(true){
        int res = index;
        int left = index * 2;
        int right = index * 2 + 1;
        if(left <= length && one->que[left] < one->que[res]){
            res = left;
        }
        if(right <= length && one->que[right] < one->que[res]){
            res = right;
        }
        if(index == res){
            break;
        }
        swap(one->que[index],one->que[res]);
        index = res;
    }
}
