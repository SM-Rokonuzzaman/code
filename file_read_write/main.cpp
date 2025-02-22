#include <iostream>
#include <fstream>
#include <string>


using namespace std;

void write_file(string file_name, string message)
{
    fstream myFile;
    myFile.open(file_name, ios::out); //ios::out -> write mode
    if(myFile.is_open())
    {
        myFile << message;
        myFile.close();
    }
}

void append_file(string file_name, string message)
{
    fstream myFile;
    myFile.open(file_name, ios::app); //ios::app -> append mode
    if(myFile.is_open())
    {
        myFile << message;
        myFile.close();
    }
}

string read_file(string file_name)
{
    fstream myFile;
    myFile.open(file_name, ios::in); //ios::in ->read mode
    if(myFile.is_open())
    {
        string line,message;
        while(getline(myFile,line))
        {
            message.append(line+"\n");
            //message.append("\n");
            //strcat(message,line);
        }
        return message;
        myFile.close();
    }
}
int main()

{
    write_file("rokon2.txt","hello this is a line\n");
    append_file("rokon2.txt","Hello this is 2nd line\n");
    cout << read_file("command.txt");
    return 0;
}
