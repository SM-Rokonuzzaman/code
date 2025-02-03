/*
    C++ program to demonstrate character arith,etic
*/

#include <iostream>
#include <string.h>
#include <stdio.h>



using namespace std;




int main()
{

    char str1[20] = "abcd", str2[20] = "ABCD", str3[20]="xyz";
strcpy(str3 + 2, strcat(str1 + 2, str2 + 1));
printf("%s.\n",str3);
     return 0;
}
