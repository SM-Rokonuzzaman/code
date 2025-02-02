#include <iostream>

int *ptr;

int value;

using namespace std;

int main()
{
    value = 10;
    ptr = &value;

    cout << "Value: " << value << endl;
    cout << "Value using pointer: " << *ptr << endl;

    cout << "Hello world!" << endl;
    return 0;
}
