/*
    C++ program to demonstrate character arithmetic
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>


using namespace std;

int main()
{
   unsigned char a = 0, b = 0, c = 0, d = 0;
a = 7 ^ 3;
b = 3 << 2;
c = ~1;
d = ~(a >> 1);
printf("%u, %u, %u, %u, %u, %u.\n", a, b, c, a & b, a || b, d);
    return 0;
}
