#include <fstream>
#include <iostream>
#include <iomanip>
#include "console.h"
#include "filelib.h"
#include "vector.h"
#include "hashmap.h"
#include "simpio.h"

using namespace std;

/*
 * 10 / 2 == 5 ..... 0
 * 5  / 2 == 2 ..... 1
 * 2  / 2 == 1 ..... 0
 * 1  / 2 == 0 ..... 1
 *
 *
 * /
*/

void printBinary(int n){
    //base case
    if(n == 0){
        return;
    }
    else if(n < 0){
        cout << "-";
        printBinary(0 - n);
    }
    else{
        int m = n % 2;
        printBinary(n / 2);
        cout << m;
    }
    //recusive case
}

void reverseLines(ifstream& input){
    string line;
    //base case
    if(!getline(input,line)){
        //
    }
    else{
        //recursive case
        reverseLines(input);
        cout << line << endl;
    }
}

void crawl(const string& filename,const string&  indentation = ""){
    cout << indentation << getTail(filename) << endl;
    //base case
    if(isFile(filename)){
        //cout << getTail(filename) << endl;
    }
    else{
        Vector<string> files = listDirectory(filename);
        for(string& file: files){
            crawl(filename +"//" + file,indentation + "     ");
        }
    }
}

int fib(HashMap<int,int>& cache,int n){
    if(n == 1 || n == 2){
        return 1;
    }
    else if(cache.containsKey(n)){
        return cache[n];
    }
    else{
        int res = fib(cache,n - 1) + fib(cache,n - 2);
        cache[n] = res;
        return res;
    }
}

int main(){
    // printBinary(-10);
    // cout << endl;
    // printBinary(10);
    // cout << endl;
    // printBinary(-42);
    // ifstream input;
    // string filename = "C://cs106x//lecture//lecture7//power";
    // input.open(filename);
    // reverseLines(input);
    // crawl(filename,"");
    HashMap<int,int> cache;
    cout << fib(cache,9);
    return 0;
}
