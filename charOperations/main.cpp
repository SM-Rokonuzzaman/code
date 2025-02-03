/*
    C++ program to demonstrate character arith,etic
*/

#include <iostream>
#include <string.h>
#include <stdio.h>



using namespace std;

char array1[2][5]={"1234","abcd"};
char (*ptr)[5] = array1;


int main()
{
    char *array3[3]={"adh","bei", "cfh"};
char **p3 = array3;
printf("%s, %s, %c, %c.\n", *p3 + 2, *(p3 + 2), **p3 + 2, **(p3 +2));

     return 0;
}
