/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */

#include <cctype>
#include <iostream>
#include <ctype.h>
#include <vector>
#include "console.h"
#include "gboggle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"
#include "grid.h"
#include "map.h"
#include "set.h"
#include "lexicon.h"
using namespace std;

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static const int kMinLength = 4;
static const double kDelayBetweenHighlights = 100;
static const double kDelayAfterAllHighlights = 500;

vector<vector<char>> getLetter(int dimension);
void drawLetters(const vector<vector<char>>& v,int dimension);
void helper(Lexicon& dictionary,vector<vector<char>>& v,vector<vector<bool>> used,set<string>& word,string& chosen,int row,int col);
set<string> getDictionary(const vector<vector<char>>& v);
char chose();
vector<vector<char>> getinput(int dimension);

/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: getPreferredBoardSize
 * -------------------------------
 * Repeatedly prompts the user until he or she responds with one
 * of the two supported Boggle board dimensions.
 */

static int getPreferredBoardSize() {
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)." << endl;
    return getIntegerBetween("Which dimension would you prefer: 4 or 5?", 4, 5);
}

/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    int dimension = getPreferredBoardSize();
    drawBoard(dimension, dimension);
    vector<vector<char>> letter;
    //labelCube(3,3,'a');
    char ch = chose();
    if(ch == 'n'){
        letter = getLetter(dimension);
    }
    else{
        letter = getinput(dimension);
    }
    drawLetters(letter,dimension);
    set<string> words = getDictionary(letter);
    string input;
    set<string> human;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while(true){
        cout << "Enter one word :";

        getline(cin,input);
        cout << "input is " << input << " ";
        if(input.empty()){
            break;
        }
        else if(words.find(input) != words.end() && human.find(input) == human.end()){
            recordWordForPlayer(input,HUMAN);
            human.insert(input);
        }
        else{
            if(input.size() < 4){
                cout << "word must more than four" << endl;
            }
            else{
                cout << "false,please input again" << endl;
            }
            input.clear();
        }
    }
    int count  = 0;
    for(string s : words){
        if(human.find(s) == human.end()){
            count++;
            recordWordForPlayer(s,COMPUTER);
        }
    }
    if(human.size() > count){
        cout << "oh,you win the game" << endl;
    }
    else{
        cout << "oh no lose" << endl;
    }
    pause(10000);
    cout << "This is where you'd play the game of Boggle" << endl;
}

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();
    do {
        playBoggle();
    } while (getYesOrNo("Would you like to play again?"));
    cout << "Thank you for playing!" << endl;
    shutdownGBoggle();
    return 0;
}

vector<vector<char>> getLetter(int dimension){
    int n = dimension * dimension;
    vector<string> v;
    vector<vector<char>> letters(dimension,vector<char>(dimension,'a'));
    if(dimension == 4){
        for(string s: kStandardCubes){
            v.push_back(s);
        }
    }
    if(dimension == 5){
        for(string s : kBigBoggleCubes){
            v.push_back(s);
        }
    }
    int row = 0;
    int col = 0;
    for(int i = 0;i < n;i++){
        int r = randomInteger(i,dimension*dimension - 1);
        swap(v[i],v[r]);
        int index = randomInteger(0,5);
        letters[row][col++] = (v[i][index]);
        if(col == dimension){
            col = 0;
            row++;
        }
    }
    return letters;
}

void drawLetters(const vector<vector<char>>& v,int dimension){
    for(int i = 0;i < dimension;i++){
        for(int j = 0;j < dimension;j++){
            labelCube(i,j,v[i][j]);
        }
    }
}


void helper(Lexicon& dictionary,const vector<vector<char>>& v,vector<vector<bool>>& used,set<string>& word,string& chosen,int row,int col){
    //base case
    if(!dictionary.containsPrefix(chosen)){
        return;
    }
    else{
        if(dictionary.contains(chosen) && chosen.size() >= 4 && word.find(chosen) == word.end()){
            //cout << chosen << " ";
            word.insert(chosen);
        }
        for(int i = row;i < v.size();i++){
            for(int j = col;j < v.size();j++){
                int index_x[3] = {i,i - 1,i + 1};
                int index_y[3] = {j,j - 1,j + 1};
                for(int n = 0;n < 3;n++){
                    for(int m = 0;m < 3;m++){
                        if(index_x[n] >= 0 && index_x[n] < v.size() &&
                            index_y[m] >= 0 && index_y[m] < v.size()){
                            int r = index_x[n];
                            int c = index_y[m];
                            //chosen
                            if(used[r][c] == false) {
                                chosen += toLowerCase(v[r][c]);
                                used[r][c] = true;
                                if(dictionary.containsPrefix(chosen)){
                                    //explore
                                    helper(dictionary,v,used,word,chosen,r,c + 1);
                                }
                                used[r][c] = false;
                                //unchosen
                                chosen.erase(chosen.size() - 1);
                            }

                        }
                    }
                }
            }
        }
    }
}

set<string> getDictionary(const vector<vector<char>>& v){
    int n = v.size();
    Lexicon dictionary("res//dictionary.txt");
    //cout << dictionary.size();
    set<string> words;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            vector<vector<bool>> used(n,vector<bool>(n,false));
            string chosen;
            helper(dictionary,v,used,words,chosen,i,j);
        }
    }
    return words;
}

char chose(){
    cout << "enter letter by yourself ";
    string ch;
    while(cin >> ch){
        if(ch == toLowerCase("y") || ch == toLowerCase("yes")){
            return 'y';
        }
        else if(ch == toLowerCase("n") || ch == toLowerCase("no")){
            return 'n';
        }
        else{
            cout << "please input again; " <<
                "please input y or n" << endl;
        }
    }
    return 'n';
}

vector<vector<char>> getinput(int dimension){
    cout << "please input letter " << endl;
    vector<vector<char>> letter(dimension,vector<char>(dimension,'a'));
    int n = dimension;
    int count = 1;
    int row = 0;
    int col = 0;
    while(n--){
        string line;
        cout << "The " << count << "line "
            << "please input " << dimension << " letters :" ;
        cin >> line;
        line = toUpperCase(line);
        while(line.size() != dimension){
            cout << "The " << count << "line "
                 << "please input " << dimension << " letters " << endl;
            cin >> line;
        }
        for(char ch : line){
            letter[row][col++] = ch;
        }
        row++;
    }
    cout << "ok" << endl;
    return letter;
}
