

#include <fstream>
#include <iostream>
#include <iomanip>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "gwindow.h"

using namespace std;

void hailstone(int n) {
    cout << n << endl;
    if(n == 1) {
        return;
    } else {
        if(n % 2 == 0) {
            // n is even so we repeat the process with n/2
            hailstone(n / 2);
        } else {
            // n is odd so we repeat the process with 3 * n + 1
            hailstone(3 * n + 1);
        }
    }
}

int fact(int n){
    if(n <= 1){
        return 1;
    }
    else{
        return n * fact(n - 1);
    }
}

bool isPalindrome(string s){
    //base case
    if(s.size() <= 1){
        return true;
    }
    else{
        //recursive case
        char first = s[0];
        char last = s[s.size() - 1];
        if(first == last){
            string middle = s.substr(1,s.size() - 2);
            return isPalindrome(middle);
        }
        else{
            return false;
        }
    }

}

void test_isPalindrome(string s){
    if(isPalindrome(s)){
        cout << "true" << endl;
    }
    else{
        cout << "false" << endl;
    }
}


void fractal(GWindow& gw,int x,int y,int size,int order) {
    if(order >= 0){
        fractal(gw,x - size/2,y - size/2,size/2,order - 1);

        fractal(gw,x + size/2,y + size/2,size/2,order - 1);

        fractal(gw,x + size/2,y - size/2,size/2,order - 1);

        fractal(gw,x - size/2,y + size/2,size/2,order - 1);
    }
    else{
        gw.fillRect(x,y,size,size);
    }
}

void cantor(GWindow& window,int x,int y,int length,int levels){
    if(levels <= 1){
        window.drawLine(x,y,x + length,y);
    }
    else{
        window.drawLine(x,y,x + length,y);
        cantor(window,x,y + 20,length/3,levels - 1);
        cantor(window,x + 2 * length/3,y + 20,length/3,levels - 1);
    }
}

int main() {
    // cout << "Recursive Hailstone" << endl;
    // while(true) {
    //     int startNumber = getInteger("Enter a start number (0 to exit): ");
    //     if(startNumber == 0) break;
    //     hailstone(startNumber);
    // }

    // test_isPalindrome("madma");
    // test_isPalindrome("racecar");
    // test_isPalindrome("X");
    // test_isPalindrome("ONE ENO");

    GWindow gw(800,600);
    //gw.setLocation(0,0);
    gw.setColor(123);
    gw.setWindowTitle("windows 106x");

    //fractal(gw,200,200,500,5);
    cantor(gw,50,50,700,7);
    return 0;
}



