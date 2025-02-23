#include <iostream>
<<<<<<< HEAD
#include <fstream>

using namespace std;

#define rowA 3
#define colA 2
#define rowB 2
#define colB 3

void write_file(string res)
{
    fstream myfile;
    myfile.open("result.txt",ios::app);
    if(myfile.is_open())
    {
        myfile << res << " ";
        myfile.close();
    }
}

int matA[rowA][colA]=   {{1,2},
                        {4,5},
                        {7,8}};

int matB[rowB][colB]=   {{1,2,3},
                        {4,5,6},
                        };

int res[3][3];

//int (*matAptr)[3]=matA;
int main()
{

    int sum = 0;
    for(int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
        {
             for(int k = 0; k < rowB; k++)
             {
                 sum+= matA[i][k]*matB[k][j];
             }
                res[i][j] = sum;
                write_file(to_string(+res[i][j])+' ');
                //cout << char(res[i][j]);
                sum = 0;

        }       write_file("\n");
    }

    for(int i = 0; i < 3; i++)
    {
        for (int j =0; j < 3; j++)
        {
            cout << res[i][j]<<" ";
        }cout << endl;
    }
=======

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
>>>>>>> 3a73c755728fe9bdc1dcfacada69e40cfb3940f8
    return 0;
}
