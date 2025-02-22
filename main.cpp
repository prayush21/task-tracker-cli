// Online C++ compiler to run C++ program online
// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <utility>

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
    Task(){}

    Task(string desc, string status = "todo")
    {
        this->id = IDGenerator::getNewID();
        this->description = desc;
        this->status = status;

        time_t now = time(0);
        this->createdAt = now;
        this->createdAt = now;
    }

    int getTaskId(){
        return this->id;
    }

    string getStatus(){
        return this->status;

    }

    string getDescription(){
        return this->description;
    }

    void getTask(){
        cout << "Task ID: " << this->id <<endl;
        cout << "Description: " << this->description <<endl;
        cout << "Status: " << this->status <<endl;
    }

    void updateTask(string task, string status){

        this->description = task;
        this->status = status;
        this->updatedAt = time(0);

        this->getTask();
    }
    // ~Task();
};

// Task::Task(/* args */)
// {
// }

// Task::~Task()
// {
// }

class TaskManager{
    private:
    vector<Task> tasks;

    public:
    TaskManager(vector<Task>& tasks) : tasks(tasks) {
        cout << "Construcutor Called" << endl;
        this->tasks = tasks;
    }

    void addTask(string desc, string status = "todo"){
        Task newTask(desc, status);
        this->tasks.push_back(newTask);
        
    }

    void updateTask(int id, string updatedDescription, string updatedStatus){
        //TODO: Mange the not found case
        int indexToUpdate = -1;
        for(int i=0; i < this->tasks.size(); i++){
            Task& task = this->tasks[i];
            if(id == task.getTaskId()){
                indexToUpdate = id;
                cout << "updating task..." << endl;
                task.updateTask(updatedDescription, updatedStatus);
            }
        }
        if(indexToUpdate == -1){
            cout << "Task ID: " << indexToUpdate << " not found" << endl;
        }
    }

    void deleteTask(int id){
        //TODO: Delete task in the tasks vector
        int indexToErase = -1;
        for(int i = 0; i < this->tasks.size(); i++){
            if(id == this->tasks[i].getTaskId()){
                indexToErase = i;
            }
        }

        if(indexToErase == -1){
            cout << "Task ID: " << id << " not present in the list" << endl;
        }else{
            cout << "Deleting Task: " << this->tasks[indexToErase].getDescription() << "..." << endl;
            this->tasks.erase(this->tasks.begin() + indexToErase);
        }

    }

    vector<Task> getAllTasks(){
        vector<Task> taskList;
        for(auto task: tasks){
            taskList.push_back(task);
        }

        return taskList;
    }

    vector<string> getTasksByStatus(string status){
        vector<string> list;
        for(auto task: this->tasks){
            if(task.getStatus() == status){
                list.push_back(task.getDescription());
            }
        }
        return list;
    }  
};

int main()
{
    // Write C++ code here
    vector<Task> tasksInput = {
        Task("Task 1"),
        Task("Task 2"),
        Task("Task 3"),
        Task("Task 4"),
        Task("Task 5"),
    };
    TaskManager taskManager(tasksInput);
    
    // vector<Task> tasksList = taskManager.getAllTasks();
    for(auto task: taskManager.getAllTasks()){
        cout << task.getTaskId() <<", "<< task.getDescription() <<", "<< task.getStatus() << endl;
    }
    taskManager.updateTask(3, "This is udpdated and done", "done");
    taskManager.deleteTask(7);
    taskManager.addTask("This is task is in progress", "in-progress");
    // tasksList = taskManager.getAllTasks();
    for(auto task: taskManager.getAllTasks()){
        cout << task.getTaskId() <<", "<< task.getDescription() <<", "<< task.getStatus() << endl;
    }

    for(auto taskDesc: taskManager.getTasksByStatus("done")){
        cout << taskDesc << endl;
    }
    return 0;
}