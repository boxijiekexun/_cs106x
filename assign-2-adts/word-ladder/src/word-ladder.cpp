/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

vector<char> letters{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

static vector<string> wordlader(const Lexicon& english, const string& start, const string& end,queue<vector<string>>& ladders){
    vector<string> ladder;
    vector<string> word;
    set<string> set;
    word.push_back(start);
    ladders.push(word);
    while(!ladders.empty()){
        //cout << "oh" << endl;
        vector<string> temp = ladders.front();
        ladders.pop();
        if(temp[temp.size() - 1] == end){
            ladder = temp;
            //cout << end << endl;
            break;
        }
        else{
            string ss = temp[temp.size() - 1];
            for(int i = 0;i < ss.size();i++){
                string s = ss;
                for(int j = 0;j < letters.size();j++){
                    s[i] = letters[j];
                    if(!english.contains(s) || set.find(s) != set.end()){
                        //cout << s << endl;
                        continue;
                    }
                    else{
                        //cout << temp.size() + 1 << " ";
                        vector<string> clone = temp;
                        clone.push_back(s);
                        ladders.push(clone);
                        set.insert(s);
                    }
                }
            }
        }
    }
    return ladder;
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    //判断是否存在word  ladder
    //存在
    //不存在
    queue<vector<string>> ladders;
    vector<string> ladder = wordlader(english,start,end,ladders);
    if(ladder.size() == 0){
        cout << "not found " << endl;
    }
    //cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    cout << "Found ladder : ";
    for(int i = 0;i < ladder.size();i++){
        cout << ladder[i] << " ";
    }
    cout << endl;
}

static const string kEnglishLanguageDatafile = "res//dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
