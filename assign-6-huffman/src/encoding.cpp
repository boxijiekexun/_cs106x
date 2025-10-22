// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
using namespace std;
// TODO: include any other headers you need


// 统计频率
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    string line;
    while(getline(input,line)){
        for(int i = 0;i < line.size();i++){
            freqTable[line[i]]++;
        }
        // freqTable[10]++; // \n num
    }
    // 统计EOF 字符
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}

// 建立树
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> queue;
    HuffmanNode* Tree = new HuffmanNode();
    for(const auto& p: freqTable){
        HuffmanNode* node = new HuffmanNode(p,freqTable[p]);
        queue.enqueue(node,freqTable[p]);
    }
    while(!queue.isEmpty()){
        while(queue.size() >= 2){
            HuffmanNode* left = queue.dequeue();
            HuffmanNode* right = queue.dequeue();
            int count = left->count + right->count;
            HuffmanNode* node = new HuffmanNode(NOT_A_CHAR,count,left,right);
            queue.enqueue(node,count);
        }
        if(queue.size() == 1){
            Tree = queue.dequeue();
        }
    }
    return Tree;
}

void buildEncodingMapHelper(HuffmanNode* encodingTree,Map<int,string>& encodingMap,string& chosen){
    if (encodingTree == nullptr) {
        return;
    }
    if(encodingTree->character != NOT_A_CHAR){
        encodingMap[encodingTree->character] = chosen;
        return;
    }
    if(encodingTree->zero != nullptr){
        chosen += '0';
        buildEncodingMapHelper(encodingTree->zero,encodingMap,chosen);
        chosen.erase(chosen.size() - 1);
    }
    if(encodingTree->one != nullptr){
        chosen += '1';
        buildEncodingMapHelper(encodingTree->one,encodingMap,chosen);
        chosen.erase(chosen.size() - 1); // 回溯
    }

}

//
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string chosen = "";
    buildEncodingMapHelper(encodingTree,encodingMap,chosen);
    return encodingMap;
}

void writeHelper(string& code,obitstream& output){
    for(char ch : code){
        if(ch == '0'){
            output.writeBit(0);
        }
        if(ch == '1'){
            output.writeBit(1);
        }
    }
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    string line;
    while(getline(input,line)){
        for(int i = 0;i < line.size();i++){
            string code = encodingMap[line[i]];
            writeHelper(code,output);
        }
    }
    string code = encodingMap[PSEUDO_EOF];
    cout << "code is " << code << endl;
    writeHelper(code,output);
}

bool decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output){
    if(encodingTree->isLeaf()){
        int ch = encodingTree->character;
        if (ch == PSEUDO_EOF) {
            return false; // 表示已到达结束标记
        }
        output.put(ch);
        return true;
    }
    int bit = input.readBit();
    cout << bit ;
    if(bit == 0){
        return decodeDataHelper(input,encodingTree->zero,output);
    }
    else{
        return decodeDataHelper(input,encodingTree->one,output);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    //rewindStream(input);
    int n = 0;
    while(decodeDataHelper(input,encodingTree,output)){
        n++;
    }
    cout << "loop is " << n << endl;
}

void compress(istream& input, obitstream& output) {
    Map<int,int> freTable = buildFrequencyTable(input);
    //input.clear(); // 清除eof标志
    HuffmanNode* Tree = buildEncodingTree(freTable);
    Map<int, string> encodingMap = buildEncodingMap(Tree);
    output << freTable;
    cout << freTable << endl;
    cout << Tree << endl;
    cout << encodingMap << endl;
    cout << "output.size is " << output.size() << endl;
    rewindStream(input);
    encodeData(input,encodingMap,output);
    freeTree(Tree);
}

void decompress(ibitstream& input, ostream& output) {
    cout << "input.size is " << input.size() << endl;
    Map<int, int> freTable;
    input >> freTable;
    cout << input.size() << endl;
    cout << freTable << endl;
    HuffmanNode* encodingTree = buildEncodingTree(freTable);
    cout << encodingTree << endl;
    decodeData(input,encodingTree,output);
}

void freeTree(HuffmanNode* node) {
    if(node == NULL){
        HuffmanNode* trash = node;
        delete trash;
    }
    else{
        freeTree(node->one);
        freeTree(node->zero);
        node->zero = NULL;
        node->one = NULL;
        HuffmanNode* trash = node;
        delete trash;
        node = NULL;
    }
}
