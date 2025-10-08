/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <random>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "set.h"
#include "maze-graphics.h"
#include "random.h"

static int getMazeDimension(string prompt,
                            int minDimension = 4, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

//draw wall

set<wall> drawWall(MazeGeneratorView& display, int dimension){
    display.drawBorder();
    display.repaint();
    display.setDimension(dimension);
    pause(1000);
    vector<cell> cells;
    set<wall> walls;
    for(int i = 0;i < dimension;i++){
        for(int j = 0;j < dimension;j++){
            cells.push_back({i,j});
        }
    }
    //draw row
    for(int i = 0;i < cells.size() - 1;i++){
        if(i + dimension < cells.size()){
            walls.insert({cells[i],cells[i + dimension]});
        }
    }
    //draw col
    for(int i = 0;i < cells.size();i++){
        if(i != 1 && i % dimension == 0){
            continue;
        }
        walls.insert({cells[i - 1],cells[i]});
    }
    display.addAllWalls(walls);
    display.repaint();
    display.repaint();
    return walls;
}

//delete wall
void setmaze(MazeGeneratorView& display,set<wall> walls,int dimension){
    int n = dimension * dimension - 1;
    Set<cell> chamber;
    Set<wall> remove;

    // obtain a time-based seed
    srand(time(nullptr)); // 初始化随机种子
    Set<cell> cells;
    for(int i = 0;i < dimension;i++){
        for(int j = 0;j < dimension;j++){
            cells.add({i,j});
        }
    }

    while(cells != chamber){
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::vector<wall> shuffledWalls(walls.begin(), walls.end());
        std::shuffle(shuffledWalls.begin(), shuffledWalls.end(), rng);
        for(wall w : shuffledWalls){
            cell first = w.one;
            cell second = w.two;
            if(!remove.contains(w)){
                if((!chamber.contains(first) && chamber.contains(second)) ||
                    (!chamber.contains(second) && chamber.contains(first))||
                    chamber.isEmpty()){
                    cout << "cnmd      " << endl;
                    if(!chamber.contains(first))
                    {
                        chamber.add(first);
                    }
                    if(!chamber.contains(second))
                    {
                        chamber.add(second);
                    }
                    walls.erase(w);
                    remove.add(w);
                    display.removeWall(w);
                    display.repaint();
                }
            }
        }
    }
    display.setDimension(dimension);
    display.addAllWalls(walls);
    display.drawBorder();
    display.repaint();
    pause(10000);

}

int main() {
    while (true) {
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;
        MazeGeneratorView display;
        set<wall> walls = drawWall(display,dimension);
        setmaze(display,walls,dimension);
    }

    return 0;
}
