/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <queue>
#include <map>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "random.h"

static const string kGrammarsDirectory = "res//grammars//";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

//getStart
string getStart(ifstream& input,string filename){
    if(!input){
        cout << "no" << endl;
    }
    input.open(filename);
    string line;
    while(getline(input,line)){
        if(line == "<start>"){
            getline(input,line);
            int n = stoi(line);
            n = randomInteger(1,n);
            int count = 0;
            while(getline(input,line)){
                count++;
                if(count == n){
                    //cout <<"start is " << line << endl;
                    return line;
                }
            }
        }
    }
    return line;
}

queue<string> dismiss(string s){
    queue<string> que;
    string str;
    // for(char ch : s){
    //     if(ch == ' ' && ch){
    //         que.push(str);
    //         str.clear();
    //     }
    //     str += ch;
    // }
    for(int i = 0;i < s.size();i++){
        if(s[i] == '<'){
            str += s[i++];
            while(s[i] != '>'){
                str += s[i++];
            }
            str += s[i];
            que.push(str);
            //cout << str << endl;
            str.clear();
            continue;
        }
        if(s[i] == ' '){
            if(str.empty()) continue;
            que.push(str);
            //cout << str << endl;
            str.clear();
            continue;
        }
        str += s[i];
    }
    if(str[str.size() - 1] != '>') que.push(str);
    return que;
}


void generateHelper(map<string,vector<string>>& dictionary,queue<string>& que,string& sentence){
    while(!que.empty()){
        string str = trim(que.front());
        que.pop();
        //cout << str << endl;
        if(dictionary.find(str) == dictionary.end()){
            //cout << str << endl;
            if(str.empty()) continue;
            sentence += str;
            sentence += " ";
        }
        else{
            vector<string> val = dictionary[str];
            int n = randomInteger(0,val.size() - 1);
            string temp = val[n];
            //cout << "temp is " << temp << endl;
            queue<string> q = dismiss(temp);
            generateHelper(dictionary,q,sentence);
        }
    }
}

void generate(map<string,vector<string>>& dictionary,string& s,string& sentence){
    //cout << "s is : " << s << endl;

    queue<string> que = dismiss(s);
    generateHelper(dictionary,que,sentence);
}


void get_dictionary(const string& file,map<string,vector<string>>& dictionary){
    ifstream input;
    input.open(file);
    string line;
    while(getline(input,line)){
        string key = line;
        vector<string> val;
        getline(input,line);
        int n = stoi(line);                               //get line size
        while(n--){
            getline(input,line);
            val.push_back(line);
        }
        dictionary[key] = val;
        getline(input,line);                              //skip empty line
    }
    //cout << "dictionary " << dictionary.size() << endl;
    // for(auto str: dictionary){
    //     cout << str.first << endl;
    //     if(dictionary.find("<Times Sq>") != dictionary.end()){
    //         vector<string> s = dictionary["<Times Sq>"];
    //         //cout << s.size() << endl;
    //     }
    // }

}


int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        string file = kGrammarsDirectory + filename;
        ifstream input;
        string start = getStart(input,file);

        map<string,vector<string>> dictionary;
        get_dictionary(file,dictionary);
        string sentence;
        int count = 3;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
        while(count--){
            generate(dictionary,start,sentence);
            cout << "sentence " << count << " : " << sentence << endl;
            sentence.clear();
        }
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
