/*
a pointer is a variable that stores the memory address of another variable.
instead of holding the direct value, it holds the address where the value is stored in the memory.

there are 2 important operators
    -Dereferencing operator (*):used to declare pointer variable and access
        the value stored in the address.
    -Address operator (&*:used to returns the address of a variable or to
        access the address of a variable to a pointer.


*/

#include <iostream>



int m = 100;    // taking an integer variable

int *ptr = &m;  // pointer variable that stores the address of m variable.

using namespace std;

int main()
{

    cout << "Value of m variable (m): " << m << endl;
    cout << "Value of m variable using (*ptr) : " << *ptr << endl;
    cout << "Address of m variable (&m)" << &m << endl;
    cout << "Address of m variable using (ptr)" << ptr << endl;



    cout << "Hello world!" << endl;
    return 0;
}
