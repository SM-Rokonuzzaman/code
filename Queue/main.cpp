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

    myQueue.push(1);
    myQueue.push(2);
    myQueue.push(3);
    myQueue.push(4);

    cout << "The size of the Queue is: " << myQueue.size() << endl;
    cout << "The first element is: " << myQueue.front() << endl;
    cout << "The last element is: " << myQueue.back() << endl;

    printQueue(myQueue);
    return 0;
}
