/*
 * CS 106X, Chris Gregg 
 * This program demonstrates how to use recursion
 * to crawl a file directory
 */
#include <fstream>
#include <iostream>
#include <iomanip>
#include "console.h"
#include "filelib.h"
#include "vector.h"
#include "simpio.h"


using namespace std;
// function prototypes
void crawl(string path);

int evaHelper(const string& exp,int& index){
    if(isdigit(exp[index])){
        return exp[index++] - '0';
    }
    else{
        index++;                          //skip
        // operand
        int left = evaHelper(exp,index);

        // operator
        char op = exp[index++];

        // operand
        int right = evaHelper(exp,index);

        // )
        index++;      //skip

        if(op == '+'){
            return left + right;
        }
        else if(op == '*'){
            return left * right;
        }
    }
    return 0;
}

int evaluate(const string& exp){
    int index = 0;
    return evaHelper(exp,index);
}

int main() {
    // string root = getLine("Please enter a directory: ");

    // cout << "looking..." << endl;

    // crawl(root);

    // cout << "done" << endl;
    cout << evaluate("((1+2)*3)");
    return 0;
}

void crawl(string path) {
    // base case
    if (isFile(path)) {
        cout << path << endl;
        return;
    }
    // recursive case
    Vector<string> contents;
    listDirectory(path); // populates contents with the directory listing
    for (string element : contents) {
        // make a recursive call for each element
        string newPath = path + "/" + element;
        crawl(newPath);
    }
}
