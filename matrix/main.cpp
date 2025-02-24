#include <iostream>
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
int (*mataptr)[2] = matA;

int matB[rowB][colB]=   {{1,2,3},
                        {4,5,6},
                        };

int res[3][3];

//int (*matAptr)[3]=matA;

struct employee
{
    int age;
    const char * name;
    int salay;
    int *matrix[2];


};
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



     struct employee e1 = {25, "Rokonuzzaman",20};
     struct employee e2 = {20, "Sowrov Das",25};


    cout << e2.age << endl;
    cout << e1.matrix << endl;



    return 0;
}
