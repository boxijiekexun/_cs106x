#include "pratice.h"
#include <vector>

// pratice::pratice() {}

#include <iostream>  // for cout
#include <fstream>
using namespace std;

void removeAll(vector<string>& v,string s){
    for(int i = v.size() - 1;i >= 0;i--){
        if(v[i] == s){
            v.erase(v.begin() + i);
        }
    }
}

void pri(vector<string>& v){
    for(string str: v){
        cout << str << " ";
    }
    cout << endl;
}


// int main(){
//     vector<string> v {"a","b","c","b","b","a","c"};
//     cout << "v is " ;
//     pri(v);
//     removeAll(v,"b");
//     cout << "v remove is " ;
//     pri(v);
//     return 0;
// }

