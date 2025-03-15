#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Student class to represent a student record
class Student {
public:
    int id;
    string name;
    int age;
    double gpa;

    Student(int id, string name, int age, double gpa)
        : id(id), name(name), age(age), gpa(gpa) {
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name
            << ", Age: " << age << ", GPA: " << gpa << endl;
    }
};

// StudentManager class to manage student records
class StudentManager {
private:
    vector<Student> students;
    map<int, Student*> studentMap; // For quick lookup by ID

public:
    // Add a new student
    void addStudent(string name, int age, double gpa) {
        int id = getNextID(); // Automatically generate the next ID
        Student newStudent(id, name, age, gpa);
        students.push_back(newStudent);
        studentMap[id] = &students.back();
        cout << "Student added successfully with ID: " << id << endl;
    }

    // Update an existing student
    void updateStudent(int id, string name, int age, double gpa) {
        if (studentMap.find(id) != studentMap.end()) {
            Student* student = studentMap[id];
            student->name = name;
            student->age = age;
            student->gpa = gpa;
            cout << "Student updated successfully!" << endl;
        }
        else {
            cout << "Student with ID " << id << " not found!" << endl;
        }
    }

    // Delete a student by ID
    void deleteStudent(int id) {
        auto it = remove_if(students.begin(), students.end(),
            [id](const Student& s) { return s.id == id; });
        if (it != students.end()) {
            students.erase(it, students.end());
            studentMap.erase(id);
            cout << "Student deleted successfully!" << endl;
            renumberIDs(); // Renumber IDs after deletion
        }
        else {
            cout << "Student with ID " << id << " not found!" << endl;
        }
    }

    // Delete a student by name
    void deleteStudentByName(const string& name) {
        vector<Student*> matches;
        for (auto& student : students) {
            if (student.name.find(name) != string::npos) {
                matches.push_back(&student);
            }
        }

        if (matches.empty()) {
            cout << "No student found with name containing \"" << name << "\"." << endl;
            return;
        }

        if (matches.size() == 1) {
            cout << "Found 1 student:" << endl;
            matches[0]->display();
            cout << "Are you sure you want to delete this student? (yes/no): ";
            string confirm;
            cin >> confirm;
            if (confirm == "yes") {
                deleteStudent(matches[0]->id);
            }
            else {
                cout << "Deletion canceled." << endl;
            }
        }
        else {
            cout << "Found " << matches.size() << " students:" << endl;
            for (const auto& student : matches) {
                student->display();
            }
            cout << "Enter the ID of the student you want to delete (or type '0' to cancel): ";
            int id;
            cin >> id;
            if (id != 0) {
                deleteStudent(id);
            }
            else {
                cout << "Deletion canceled." << endl;
            }
        }
    }

    // Display all students
    void displayAllStudents() const {
        if (students.empty()) {
            cout << "No students found!" << endl;
            return;
        }
        for (const auto& student : students) {
            student.display();
        }
    }

    // Search for a student by ID
    void searchStudent(int id) const {
        if (studentMap.find(id) != studentMap.end()) {
            studentMap.at(id)->display();
        }
        else {
            cout << "Student with ID " << id << " not found!" << endl;
        }
    }

    // Search for a student by name (first or last name)
    void searchStudentByName(const string& name) const {
        bool found = false;
        for (const auto& student : students) {
            if (student.name.find(name) != string::npos) {
                student.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No student found with name containing \"" << name << "\"." << endl;
        }
    }

    // Load student data from a CSV file
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            int id, age;
            double gpa;
            string name;
            char comma;
            ss >> id >> comma;
            getline(ss, name, ',');
            ss >> age >> comma >> gpa;
            addStudent(name, age, gpa); // Automatically assign ID
        }
        cout << "Data loaded successfully from " << filename << "." << endl;
    }

    // Save student data to a CSV file
    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        file << "ID,Name,Age,GPA\n";
        for (const auto& student : students) {
            file << student.id << "," << student.name << ","
                << student.age << "," << student.gpa << "\n";
        }
        cout << "Data saved successfully to " << filename << "." << endl;
    }

private:
    // Get the next available ID
    int getNextID() {
        if (students.empty()) {
            return 1;
        }
        return students.back().id + 1;
    }

    // Renumber IDs sequentially after deletion
    void renumberIDs() {
        studentMap.clear();
        for (size_t i = 0; i < students.size(); ++i) {
            students[i].id = i + 1;
            studentMap[students[i].id] = &students[i];
        }
    }
};

// Function to display the menu
void displayMenu() {
    cout << "\nStudent Management System\n";
    cout << "1. Add Student\n";
    cout << "2. Update Student\n";
    cout << "3. Delete Student\n";
    cout << "4. Display All Students\n";
    cout << "5. Search Student by ID\n";
    cout << "6. Search Student by Name\n";
    cout << "7. Load Students from File\n";
    cout << "8. Save Students to File\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

// Function to validate integer input
int getValidIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            break;
        }
        else {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input! Please enter a valid number." << endl;
        }
    }
    return value;
}

// Function to handle "Cancel" input
bool checkForCancel(const string& input) {
    if (input == "cancel") {
        cout << "Operation canceled. Returning to main menu." << endl;
        return true;
    }
    return false;
}

// Main function
int main() {
    StudentManager manager;

    // Automatically load students.csv when the program starts
    manager.loadFromFile("students.csv");

    int choice;
    string filename;

    while (true) {
        displayMenu();
        choice = getValidIntInput("");

        switch (choice) {
        case 1: { // Add Student
            string name;
            int age;
            double gpa;

            cout << "Enter Name (or type 'cancel' to exit): ";
            cin.ignore();
            getline(cin, name);
            if (checkForCancel(name)) break;

            age = getValidIntInput("Enter Age: ");
            cout << "Enter GPA: ";
            cin >> gpa;

            manager.addStudent(name, age, gpa);
            break;
        }
        case 2: { // Update Student
            int id = getValidIntInput("Enter ID of the student to update (or type '0' to cancel): ");
            if (id == 0) break;

            string name;
            int age;
            double gpa;

            cout << "Enter new Name (or type 'cancel' to exit): ";
            cin.ignore();
            getline(cin, name);
            if (checkForCancel(name)) break;

            age = getValidIntInput("Enter new Age: ");
            cout << "Enter new GPA: ";
            cin >> gpa;

            manager.updateStudent(id, name, age, gpa);
            break;
        }
        case 3: { // Delete Student
            cout << "Delete by:\n1. ID\n2. Name\nEnter your choice: ";
            int deleteChoice;
            cin >> deleteChoice;
            if (deleteChoice == 1) {
                int id = getValidIntInput("Enter ID of the student to delete (or type '0' to cancel): ");
                if (id == 0) break;
                manager.deleteStudent(id);
            }
            else if (deleteChoice == 2) {
                string name;
                cout << "Enter name of the student to delete (or type 'cancel' to exit): ";
                cin.ignore();
                getline(cin, name);
                if (checkForCancel(name)) break;
                manager.deleteStudentByName(name);
            }
            else {
                cout << "Invalid choice!" << endl;
            }
            break;
        }
        case 4: // Display All Students
            manager.displayAllStudents();
            break;
        case 5: { // Search Student by ID
            int id = getValidIntInput("Enter ID of the student to search (or type '0' to cancel): ");
            if (id == 0) break;
            manager.searchStudent(id);
            break;
        }
        case 6: { // Search Student by Name
            string name;
            cout << "Enter name (first or last) to search (or type 'cancel' to exit): ";
            cin.ignore();
            getline(cin, name);
            if (checkForCancel(name)) break;
            manager.searchStudentByName(name);
            break;
        }
        case 7: { // Load Students from File
            cout << "Enter filename to load data from (or type 'cancel' to exit): ";
            cin >> filename;
            if (checkForCancel(filename)) break;
            manager.loadFromFile(filename);
            break;
        }
        case 8: { // Save Students to File
            cout << "Enter filename to save data to (or type 'cancel' to exit): ";
            cin >> filename;
            if (checkForCancel(filename)) break;
            manager.saveToFile(filename);
            break;
        }
        case 9: // Exit
            cout << "Exiting the program. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}