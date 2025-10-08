/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>
#include <vector> //for vector
#include <QDir>
#include <QStringList>
#include <QDebug>
using namespace std;

#include <qstring.h>
#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevent.h" // for mouse event detection
#include "strlib.h"
#include "random.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay
#include "grid.h"

//chosen file or hand
char choose();
//chosen file
Grid<char> chosen_file(char chosen);

//help debugger
void print(Grid<char> grid);

//find next generrage
Grid<char> Change(Grid<char> cur);

//update age
void Count_Age(Grid<char>& cur,Grid<char>& next,Grid<int>& age);

//get file name
vector<string> listFilesInDirectory(const QString &folderPath);

//random get file
string chosen_randoms();




/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}



/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    //display.setTitle("Game of Life");
    welcome();
    // Grid<char> grid;
    // char chosen = choose();//

    // grid = chosen_file(chosen);

    // int row = grid.numRows();
    // int col = grid.numCols();
    // Grid<int> age(row,col,1);
    // Grid<char> next = grid;
    // //display.printBoard();
    // //display.setDimensions(row,col);
    // int year = 0;

    while(1){
        Grid<char> grid;
        char chosen = choose();//

        grid = chosen_file(chosen);

        int row = grid.numRows();
        int col = grid.numCols();
        Grid<int> age(row,col,1);
        Grid<char> next = grid;
        //display.printBoard();
        //display.setDimensions(row,col);
        int year = 0;
        while(1){
            display.setTitle("Game of life");
            display.setDimensions(row,col);
            for(int i = 0;i < row;i++){
                for(int j = 0;j < col;j++){
                    if(next[i][j] == 'X'){
                        display.drawCellAt(i,j,age[i][j]);
                    }
                    //cout << next[i][j];
                }
                //cout << endl;
            }
            pause(100);
            display.repaint();
            grid = next;
            next = Change(grid);
            Count_Age(grid,next,age);
            year++;
            if(grid == next){
                cout << "year is " << year << endl;
                break;
            }
        }

        char ch;
        do{
            cout << "You can continue,or exit" << endl;
            cout << "If you chosen continue,Please input C" << endl;
            cout << "exit ,please input D" << endl;
            cin >> ch;
            ch = toupper(ch);
            ch = toupper(ch);
            cout << "chose is " << ch << endl;
        }while(ch != 'C' && ch != 'D');//失误了
        if(ch == 'C'){
            //
        }
        else{
            break;
        }

    }

    display.repaint();
    pause(10000);
    return 0;
}


char choose(){
    char chose = '\0';
    cout << "hi man ! what do you like"
         << "you can choose random or file" << endl;
    do{
        cout << "A: random           B:FILE" << endl;
        cin >> chose;
        chose = toupper(chose);
        cout << "chose is " << chose << endl;
    }while(chose != 'A' && chose != 'B');//失误了
    return chose;
}

Grid<char> chosen_file(char chosen){
    //
    ifstream input;
    string path = "res//files//";

    string filename;

    if(chosen == 'B'){
        do{
            cout << "Please string the right filename" << endl;
            cin >> filename;
            filename = path + filename;
            input.open(filename);
        }while(!input);
    }
    else if(chosen == 'A'){
        input.open(path + chosen_randoms());
    }

    string line;
    int row = 0;//行
    int col = 0;//列

    Grid<char> grid(row,col);
    while(getline(input,line)){
        cout << line << endl;
        if(line[0] == '#') continue;
        else if(line[0] >= '0' && line[0] <= '9'){
            cout << "line is " << line << endl;
            if(row == 0){
                row = stoi(line);
                cout << "row is " << row << endl;
            }
            else if(col == 0){
                col = stoi(line);
                cout << "col is " << col << endl;
            }
            if(row != 0 && col != 0){
                grid.resize(row,col);
                row = 0;
            }
        }
        else if(line[0] == '-'){
            // cout << grid.size() << "grid[0].size is " << grid[0].size() << endl;
            // cout << "line is " << line.size() << endl;
            // cout << "col is " << col << endl;
            for(int k = 0;k < line.size();k++){
                grid[row][k] = line[k];
            }
            row++;
        }
    }
    //pause(100000);
    input.close();
    return grid;
}

void print(Grid<char> grid){
    for(int i = 0;i < grid.numRows();i++){
        for(int j = 0;j < grid.numCols();j++){
            cout << grid[i][j];
        }
        cout << endl;
    }
}

Grid<char> Change(Grid<char> cur){
    Grid<char> next(cur.numRows(),cur.numCols());
    for(int i = 0;i < cur.numRows();i++){
        for(int j = 0;j < cur.numCols();j++){
            int count = 0;
            int x_index[3] = {i - 1 ,i ,i + 1};
            int y_index[3] = {j - 1 ,j ,j + 1};
            //cout << "oh no" << endl;
            for(int m = 0;m < 3;m++){
                for(int n = 0;n < 3;n++){
                    if(x_index[m] >= 0 && y_index[n] >= 0 && x_index[m] < cur.numRows() && y_index[n] < cur.numCols()){
                        int x = x_index[m];
                        int y = y_index[n];
                        if(cur[x][y] == 'X'){
                            count++;
                        }
                    }
                }
            }
            //cout << count << " " << endl;
            if(cur[i][j] == 'X'){
                count--;
            }
            if(count <= 1){
                next[i][j] = '-';
            }
            else if(count == 2){
                next[i][j] = cur[i][j];
            }
            else if(count == 3){
                next[i][j] = 'X';
            }
            else if(count > 3){
                next[i][j] = '-';
            }
            count = 0;
        }
    }
    //cout << "this is next :" << endl;
    //print(next);
    return next;
}

void Count_Age(Grid<char>& cur,Grid<char>& next,Grid<int>& age){
    for(int i = 0;i < cur.numRows();i++){
        for(int j = 0;j < cur.numCols();j++){
            if(cur[i][j] == '-' && next[i][j] == 'X'){
                age[i][j] = 1;
            }
            if(cur[i][j] == 'X' && next[i][j] == 'X'){
                age[i][j] += 1;
            }
            else{
                age[i][j] = 0;
            }
            if(age[i][j] > kMaxAge){
                age[i][j] = 0;
                next[i][j] = '-';
            }
        }
    }
}

string chosen_randoms(){
    QString paths = "res//files";
    vector<string> files = listFilesInDirectory(paths);
    int high = files.size() - 1;
    int n = randomInteger(0,high);
    string str = files[n];
    return str;
}

vector<string> listFilesInDirectory(const QString &folderPath) {

    vector<string> files;

    cout << "this random file : " << endl;
    QDir directory(folderPath);

    // 设置过滤器，只获取文件（不包含目录）
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    // 获取文件夹中所有文件
    QStringList fileList = directory.entryList();

    // 遍历并打印每个文件名
    foreach (QString fileName, fileList) {
        //cout << "oh no " << endl;
        string str = fileName.toStdString();
        files.push_back(str);
    }
    return files;
}

