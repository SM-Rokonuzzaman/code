#include <iostream>

using namespace std;
// Abstract class
class AbstractEmployee {
    virtual void AskForPromotion() = 0;
    virtual bool AskForLeave() = 0;
    virtual string note() = 0;

};

class Employee:AbstractEmployee{
private:
    string Name;
    string Company;
    int Age;
public:
    // Setter for name
    void setName(string name){
        Name  = name;
    }
    //getter for name
    string getName(){
        return Name;
    }

    // Setter for company
    void setCompany(string company){
        Company  = company;
    }
    //getter for company
    string getCompany(){
        return Company;
    }
    // Setter for age
    void setAge(int age){
        // Validation rule example
        if(age >= 18)
        Age  = age;
    }
    //getter for age
    int getAge(){
        return Age;
    }

    void IntroduceYourself(){
    cout << "Name - " << Name << endl;
    cout << "Company - " << Company << endl;
    cout << "Age - " << Age << endl;
    }

    //Constructor//
    Employee(string name, string company, int age){
    Name = name;
    Company = company;
    Age = age;
    }
    void AskForPromotion(){
        if (Age>30)
            cout << Name << " got promoted " << endl;
        else
            cout << Name << " sorry! no promotion" << endl;
    }

    bool AskForLeave(){
        return true;

    }
    string note(){
        return "Hello";

    }
};
int main()
{
    Employee employee1 = Employee("Rokon","Data edge", 26);
    Employee employee2 = Employee("Partha","Data edge", 31);
    employee1.AskForPromotion();
    employee2.AskForPromotion();

    if(employee2.AskForLeave())
        cout << "You got leave!" << endl;
        cout << employee1.note() << endl;


}
