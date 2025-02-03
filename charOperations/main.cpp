/*
    C++ program to demonstrate character arith,etic
*/

#include <iostream>
#include <string.h>
#include <stdio.h>

char array1[] = {"I am a student"};
char array2[] = "I am a student";
char *array3 = "I am a student";
char *array4[6] = {"12345","23456","34567"};







using namespace std;

int main()
{
    cout << array1 << endl;

    cout << "The size of the array with null character: " <<sizeof(array1) << endl;
    cout << "Number of characters: " << strlen(array1) << endl;

    cout << sizeof(array2) << endl;  // output is 15
    cout << strlen(array2) << endl;  // output is 14

    cout << sizeof(array3) << endl;  // output is 8 as it shows the pointer size
    cout << strlen(array3) << endl;  // output is 14

    cout << sizeof(array4) << endl; // output is 6*8 = 48, size of 6 pointers.
    cout << array4[1] << endl;
    /*
    When used with arrays, sizeof operator returns the size in bytes occupied by
    the entire array whereas when used with pointers, it returns the size in bytes
    of the pointer itself regardless of the data types it points to.
    */

    return 0;
}
