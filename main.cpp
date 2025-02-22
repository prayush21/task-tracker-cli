// Online C++ compiler to run C++ program online
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class IDGenerator
{
private:
    static int counter;

public:
    static int getNewID()
    {
        return counter++;
    }
};
int IDGenerator::counter = 1; // Initialize counter

class Task
{
private:
    int id;
    string description;
    string status;
    time_t createdAt;
    time_t updatedAt;

public:
    Task(string desc, string status = "todo")
    {
        this->id = IDGenerator::getNewID();
        this->description = desc;
        this->status = status;
    }
    // ~Task();
};

// Task::Task(/* args */)
// {
// }

// Task::~Task()
// {
// }

int main()
{
    // Write C++ code here
    Task task1("this is task 1", "todo");

    return 0;
}