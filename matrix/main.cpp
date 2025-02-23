#include <iostream>

using namespace std;

int matxA[3][3] = {{3, 12, 4},{4, 6, 8},{1, 0, 2}};
int matxB[3][3] = {{7, 3, 8},{11, 9, 5},{6, 8, 4}};
int (*ptrA)[3]= matxA;
int (*ptrB)[3]= matxB;


void print_matx(int *point )
{

    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << *point << ",";
        }
        cout << endl;
    }
}


int main()
{
    print_matx(*ptrA);
    return 0;
}
