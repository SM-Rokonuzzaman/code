#include <iostream>
#include <math.h>
using namespace std;
int m;

class car {

    public:

    int get_speed(int speed)
    {
        //cout << "The speed is: " << speed << endl;
        return speed;
    }

    void get_milage(int milage)
    {
        cout << "The milage is: " << milage << endl;
    }
    void get_input()
    {
        cout << "Enter a number: " << endl;
        cin >> m;
    }

};

class engine:public car   //the class: engine derive its properties from another class: car.
{
public:
    void engine_cc()
    {
        cout << "Engine is 1500cc" << endl;
    }
};

int main()
{
    car data;           // creating "car" class object "Data"
    engine cc;          // creating "engine" class object "cc".
    data.get_input();   // calling a function of class car
    cout << "The speed is: "<< data.get_speed(m) << endl;

    cc.engine_cc();     // calling a function of engine
    cc.get_milage(550); // calling a function of car. as engine is child class of car it is possible.
    return 0;

}
