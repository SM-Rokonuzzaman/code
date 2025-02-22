#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main()

{
    FILE* fptr;

    // Creates a file "demo_file"
    // with file access as write mode
    fptr = fopen("demo_file.txt", "w+");

      //
    fprintf(fptr, "%s", "GeeksforGeeks");
    fclose(fptr);


    FILE *file = fopen("demo_file.txt", "a");
    fprintf(file,"\nAppending text\n");
    fclose(file);


    return 0;
}
