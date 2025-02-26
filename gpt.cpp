#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

enum class TaskStatus {
    TODO,
    IN_PROGRESS,
    DONE
};

// Helper functions for TaskStatus enum
string statusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::TODO: return "todo";
        case TaskStatus::IN_PROGRESS: return "inprogress";
        case TaskStatus::DONE: return "done";
        default: throw runtime_error("Invalid status");
    }
}

TaskStatus stringToStatus(const string& status) {
    if (status == "todo") return TaskStatus::TODO;
    if (status == "inprogress") return TaskStatus::IN_PROGRESS;
    if (status == "done") return TaskStatus::DONE;
    throw runtime_error("Invalid status string: " + status);
}

class Task {
private:
    int id;
    string description;
    TaskStatus status;
    time_t createdAt;
    time_t updatedAt;

public:
    Task(int id, string desc, TaskStatus status = TaskStatus::TODO) 
        : id(id), description(desc), status(status) {
        time_t now = time(0);
        createdAt = now;
        updatedAt = now;
    }

    // Getters
    int getId() const { return id; }
    string getDescription() const { return description; }
    TaskStatus getStatus() const { return status; }
    
    // Update task
    void update(const string& desc, TaskStatus newStatus) {
        description = desc;
        status = newStatus;
        updatedAt = time(0);
    }

    // Format task for file storage
    string toFileFormat() const {
        return description + "-" + statusToString(status);
    }
};

class TaskManager {
private:
    vector<Task> tasks;
    string filename;
    int nextId;

    void saveToFile() const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Could not open file for writing: " + filename);
        }
        
        for (const auto& task : tasks) {
            file << task.toFileFormat() << endl;
        }
    }

    void loadFromFile() {
        tasks.clear();
        nextId = 1;
        
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Could not open file: " + filename);
        }

        string line;
        while (getline(file, line)) {
            size_t delimiterPos = line.find('-');
            if (delimiterPos == string::npos) continue;

            string description = line.substr(0, delimiterPos);
            string statusStr = line.substr(delimiterPos + 1);
            
            // Remove any trailing whitespace or newlines
            statusStr.erase(remove_if(statusStr.begin(), statusStr.end(), ::isspace), statusStr.end());
            
            try {
                TaskStatus status = stringToStatus(statusStr);
                tasks.emplace_back(nextId++, description, status);
            } catch (const runtime_error& e) {
                cerr << "Warning: Skipping invalid task entry: " << line << endl;
            }
        }
    }

public:
    explicit TaskManager(const string& fname) : filename(fname), nextId(1) {
        loadFromFile();
    }

    void addTask(const string& description, const string& status = "todo") {
        try {
            TaskStatus taskStatus = stringToStatus(status);
            tasks.emplace_back(nextId++, description, taskStatus);
            saveToFile();
        } catch (const runtime_error& e) {
            throw runtime_error("Failed to add task: " + string(e.what()));
        }
    }

    void updateTask(int id, const string& description, const string& status) {
        auto it = find_if(tasks.begin(), tasks.end(),
                         [id](const Task& task) { return task.getId() == id; });
        
        if (it == tasks.end()) {
            throw runtime_error("Task not found with ID: " + to_string(id));
        }

        try {
            TaskStatus taskStatus = stringToStatus(status);
            it->update(description, taskStatus);
            saveToFile();
        } catch (const runtime_error& e) {
            throw runtime_error("Failed to update task: " + string(e.what()));
        }
    }

    void deleteTask(int id) {
        auto it = find_if(tasks.begin(), tasks.end(),
                         [id](const Task& task) { return task.getId() == id; });
        
        if (it == tasks.end()) {
            throw runtime_error("Task not found with ID: " + to_string(id));
        }

        tasks.erase(it);
        saveToFile();
    }

    void listTasks(const string& statusFilter = "") {
        for (const auto& task : tasks) {
            if (statusFilter.empty() || statusToString(task.getStatus()) == statusFilter) {
                cout << "ID: " << task.getId() 
                     << " | Status: " << statusToString(task.getStatus())
                     << " | Description: " << task.getDescription() << endl;
            }
        }
    }
};

void printUsage() {
    cout << "Usage: taskmanager <command> [arguments]\n"
         << "Commands:\n"
         << "  add <description> [status]     - Add a new task\n"
         << "  update <id> <description> <status> - Update a task\n"
         << "  delete <id>                    - Delete a task\n"
         << "  list [status]                  - List all tasks or filter by status\n"
         << "Status options: todo, inprogress, done\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    try {
        TaskManager manager("example.txt");
        string command = argv[1];

        if (command == "add" && argc >= 3) {
            string status = (argc > 3) ? argv[3] : "todo";
            manager.addTask(argv[2], status);
        }
        else if (command == "update" && argc >= 5) {
            manager.updateTask(stoi(argv[2]), argv[3], argv[4]);
        }
        else if (command == "delete" && argc >= 3) {
            manager.deleteTask(stoi(argv[2]));
        }
        else if (command == "list") {
            string statusFilter = (argc > 2) ? argv[2] : "";
            manager.listTasks(statusFilter);
        }
        else {
            printUsage();
            return 1;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}