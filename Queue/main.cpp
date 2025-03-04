#include <iostream>
#include <queue>

using namespace std;

void printQueue(queue<int>queue)
{
    while(!queue.empty())
    {
        cout << queue.front() << " ";
        queue.pop();
    }
    cout << endl;
}
int main()
{
    //push(), pop(),size(), front(),back()
    queue<int>myQueue;
    queue<uint8_t>myQ;

    myQueue.push(1);
    myQueue.push(2);
    myQueue.push(3);
    myQueue.push(4);

    myQ.push(10);
    myQ.push(20);
    myQ.push(30);

    cout << "The size of the Queue is: " << myQueue.size() << endl;
    cout << "The first element is: " << myQueue.front() << endl;
    cout << "The last element is: " << myQueue.back() << endl;

    printQueue(myQueue);
    while(!myQ.empty())
    {
        printf("Element : %d\n",myQ.front());
        myQ.pop();
    }
    return 0;
}
