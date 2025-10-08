

#include <fstream>
#include <iostream>
#include <iomanip>
#include "console.h"
#include "filelib.h"
#include "simpio.h"

using namespace std;

void testPower(int base, int exp, double expected);

double power(int base, int exponent) {
    //base case
    if(exponent == 0){
        return 1;
    }
    else if(exponent < 0){
        //recursion case1
        return 1.0 / power(base,0 - exponent);
    }
    else if(exponent % 2 == 0){
        //recursion case2
        //return base * power(base,exponent - 1);
        return power(base * base,exponent / 2);
    }
    else{
        return base * power(base,exponent - 1);
    }
}



int main_power() {
    cout << "Recursive power" << endl;
    testPower(2, 5, 32);
    testPower(5, 5, 3125);
    testPower(0, 6, 0);
    testPower(-6, 3, -216);
    testPower(6, 0, 1);
    testPower(2, -3, 0.125);
    cout << "Done!" << endl;
    return 0;
}

void testPower(int base, int exponent, double expected) {
    cout << "testPower(" << base << ", " << exponent << "): " << flush;
    double result = power(base, exponent);
    cout << "\t" << result;
    if(result == expected) {
        cout << "\t[passed]" << endl;
    } else {
        cout << "\t[failed]" << endl;
    }
}

