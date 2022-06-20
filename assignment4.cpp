/*
 * Name: Ryan Chua
 * Class: assignment4.cpp
 * Test class
 */
#include <iostream>
#include <cstdlib>
#include <cassert>

#include "SkipList.h"

using namespace std;

void Test1(){
    SkipList s(5);
    cout << s;
    s.Add(4);
    cout << "After adding 4: " << endl;
    cout << s;
    s.Add(6);
    cout << "After adding 6: " << endl;
    cout << s;
    s.Add(4);
    cout << "After adding 4: " << endl;
    cout << s;
    s.Add(5);
    cout << "After adding 5: " << endl;
    cout << s;

    if(s.Contains(6)){
        cout << "Data is in the list" << endl;
    }
    else
        cout << "Data is not in the list" << endl;
    
    s.Remove(6);
    cout << "After removing 6: " << endl;
    cout << s;

    if(s.Contains(6)){
        cout << "Data is in the list" << endl;
    }
    else
        cout << "Data is not in the list" << endl;
}

void Test2(){
    //TEST PRIVATE VARIABLES
    SkipList s(5);

    s.testPrivateMethods();
    cout << "again" << endl;
    s.testPrivateMethods();
    cout << "again" << endl;
    s.testPrivateMethods();
}
void Test4(){
    SkipList s(6);
    for(int i = 0; i < 50; ++i){
        int number = rand() % 100;
        s.Add(number);
        cout << "After adding " << number << endl;
        cout << s << endl;
    }
    if(s.Contains(29)){
        cout << "Data is in the list" << endl;
    }
    else
        cout << "Data is not in the list" << endl;
    
    s.Remove(29);
    cout << "After removing 29: " << endl;
    cout << s;

    if(s.Contains(29)){
        cout << "Data is in the list" << endl;
    }
    else
        cout << "Data is not in the list" << endl;
}

void TestAll(){
    Test1();
    Test2();
    Test4();
}

int main(){
    TestAll();
    return 0;
}