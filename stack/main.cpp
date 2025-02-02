#include <iostream>
#include <stack>
using namespace std;
void printStackElement(stack<int>stack)
{
    while(!stack.empty())
    {
        cout << stack.top() << endl;
        stack.pop();
    }
}
int main()
{
    // .empty(), .size(), .push(), .pop(), .top()
    stack<int>numbersStack;

    numbersStack.push(5);   // pushes new element at the top of the stack
    numbersStack.push(6);
    numbersStack.push(7);
    printStackElement(numbersStack);


    /*cout << "Top element of the stack is: " << numbersStack.top() << endl;

    if(numbersStack.empty())
    {
        cout<< "Stack is empty" << endl;;
    }
    else
    {
        cout << "Stack is not empty" << endl;;
    }
    cout << "Stack size is " << numbersStack.size();*/
    return 0;
}
