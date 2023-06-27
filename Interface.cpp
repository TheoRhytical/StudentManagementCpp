#include <ncurses.h>
#include <string>
#include <vector>

void printMenu() {
    printw("Welcome to the Student Management System!\n\n");
    printw("Menu:\n");
    printw("1. Add student information\n");
    printw("2. Add subject grade\n");
    printw("3. Save student data to file\n");
    printw("4. Load student data from file\n");
    printw("5. Calculate statistics\n");
    printw("6. Exit\n\n");
    printw("Enter your choice: ");
}

void addStudentInformation(std::vector<Student>& students) {
    clear();
    printw("Add Student Information:\n\n");

    Student student;

    printw("Enter student name: ");
    echo();
    char name[256];
    getstr(name);
    noecho();
    student.name = name;

    printw("Enter student age: ");
    scanw("%d", &student.age);

    students.push_back(student);

    printw("\nStudent information added successfully!\n");
    getch(); // Wait for user input
}

void displayStudents(const std::vector<Student>& students) {
    clear();
    printw("Student Information:\n\n");

    for (const auto& student : students) {
        printw("Name: %s\n", student.name.c_str());
        printw("Age: %d\n\n", student.age);
    }

    printw("Press any key to continue...\n");
    getch(); // Wait for user input
}

int main() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Disable echoing of characters

    int choice;
    bool running = true;

    while (running) {
        clear(); // Clear the screen
        printMenu(); // Display the menu

        scanw("%d", &choice); // Get user input

        switch (choice) {
            case 1:
                addStudentInformation(students);
                break;

            case 2:
                // Logic to add subject grade
                break;

            case 3:
                // Logic to save student data to file
                break;

            case 4:
                // Logic to load student data from file
                break;

            case 5:
                // Logic to calculate statistics
                break;

            case 6:
                running = false;
                break;

            default:
                printw("Invalid choice! Please try again.\n");
                break;
        }

        refresh(); // Update the screen
    }

    endwin(); // Clean up ncurses

    return 0;
}
