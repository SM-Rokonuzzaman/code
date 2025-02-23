/*
    C++ program to demonstrate character arithmetic
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <string>


using namespace std;


char array1[2][5]={"1234","abcd"};
char (*ptr)[5] = array1;
char *array5[3][3] =   {{"ABCD","EFGH","HIJK"},
                        {"1234","2345","3456"},
                        {"abcd","bcde","cdef"}};

const char *myArray (const char *array5)
{
    const char *myarray = array5;
    cout << myarray << endl;
    return myarray;
}




int main()
{
   unsigned char a = 0, b = 0, c = 0, d = 0;
a = 7 ^ 3;
b = 3 << 2;
c = ~1;
d = ~(a >> 1);
printf("%u, %u, %u, %u, %u, %u.\n", a, b, c, a & b, a || b, d);
for(int i = 0; i<3; i++)
{
    for(int j = 0; j<3;j++)
    {
        printf(array5[i][j]);
        printf(" ");
    }
    printf("\n");
}
    cout << myArray ("Hello!!\n");
    cout << ptr << endl;
    return 0;

}
