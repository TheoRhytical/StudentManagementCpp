#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>


struct Subject {
    std::string name;
    double grade;
    double weight;
};

struct Student {
    std::string name;
    int age;
    std::string gender;
    std::string course;
    std::vector<Subject> subjects;
};

void printMenu() {
    printw("Welcome to the Student Management System!\n\n");
    printw("Menu:\n");
    printw("1. Add student information\n");
    printw("2. Add subject grade\n");
    printw("3. Save student data to file\n");
    printw("4. Load student data from file\n");
    printw("5. Calculate statistics\n");
    printw("6. Display student information\n");
    printw("7. Exit\n\n");
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

    printw("Enter student gender: ");
    echo();
    char gender[256];
    getstr(gender);
    noecho();
    student.gender = gender;

    printw("Enter student course: ");
    echo();
    char course[256];
    getstr(course);
    noecho();
    student.course = course;

    students.push_back(student);

    printw("\nStudent information added successfully!\n");
    getch(); // Wait for user input
}

void addSubjectGrade(std::vector<Student>& students) {
    clear();
    printw("Add Subject Grade:\n\n");

    printw("Enter student name: ");
    echo();
    char name[256];
    getstr(name);
    noecho();

    bool found = false;
    for (auto& student : students) {
        if (student.name == name) {
            found = true;

            printw("Enter subject name: ");
            char subject[256];
            getstr(subject);

            double grade;
            printw("Enter subject grade: ");
            scanw("%lf", &grade);

            double weight;
            printw("Enter subject weight: ");
            scanw("%lf", &weight);

            Subject subjectObj;
            subjectObj.name = subject;
            subjectObj.grade = grade;
            subjectObj.weight = weight;

            student.subjects.push_back(subjectObj);

            printw("\nSubject grade added successfully for %s!\n", student.name.c_str());
            break;
        }
    }

    if (!found) {
        printw("\nStudent not found!\n");
    }

    getch(); // Wait for user input
}

void saveStudentDataToFile(const std::vector<Student>& students) {
    clear();
    printw("Save Student Data to File:\n\n");

    std::string filename;
    printw("Enter the file name: ");
    echo();
    char input[256];
    getstr(input);
    noecho();
    filename = input;

    std::ofstream file(filename);

    if (!file.is_open()) {
        printw("\nFailed to open the file!\n");
    } else {
        for (const auto& student : students) {
            file << "Name: " << student.name << "\n";
            file << "Age: " << student.age << "\n";
            file << "Gender: " << student.gender << "\n";
            file << "Course: " << student.course << "\n";

            if (!student.subjects.empty()) {
                file << "Subjects:\n";
                for (const auto& subject : student.subjects) {
                    file << "Subject: " << subject.name << ", Grade: " << subject.grade << ", Weight: " << subject.weight << "\n";
                }
            }

            file << "\n";
        }

        printw("\nStudent data saved to the file successfully!\n");
    }

    file.close();
    getch(); // Wait for user input
}

void loadStudentDataFromFile(std::vector<Student>& students) {
    clear();
    printw("Load Student Data from File:\n\n");

    std::string filename;
    printw("Enter the file name: ");
    echo();
    char input[256];
    getstr(input);
    noecho();
    filename = input;

    std::ifstream file(filename);

    if (!file.is_open()) {
        printw("\nFailed to open the file!\n");
    } else {
        students.clear(); // Clear existing student data

        std::string line;
        Student student;

        while (std::getline(file, line)) {
            if (line.substr(0, 5) == "Name:") {
                student.name = line.substr(6);
            } else if (line.substr(0, 4) == "Age:") {
                student.age = std::stoi(line.substr(5));
            } else if (line.substr(0, 7) == "Gender:") {
                student.gender = line.substr(8);
            } else if (line.substr(0, 7) == "Course:") {
                student.course = line.substr(8);
            } else if (line == "Subjects:") {
                std::string subject;
                double grade;
                double weight;

                while (std::getline(file, line) && !line.empty()) {
                    subject = line.substr(line.find("Subject:") + 9);
                    grade = std::stod(line.substr(line.find("Grade:") + 7));
                    weight = std::stod(line.substr(line.find("Weight:") + 8));

                    Subject subjectObj;
                    subjectObj.name = subject;
                    subjectObj.grade = grade;
                    subjectObj.weight = weight;

                    student.subjects.push_back(subjectObj);
                }
            } else if (line.empty()) {
                students.push_back(student);
                student = Student();
            }
        }

        printw("\nStudent data loaded from the file successfully!\n");
    }

    file.close();
    getch(); // Wait for user input
}

void calculateStatistics(const std::vector<Student>& students) {
    clear();
    printw("Grade Statistics:\n\n");

    if (students.empty()) {
        printw("No students available!\n");
    } else {
        std::vector<double> allGrades;
        double totalGradePoints = 0.0;

        for (const auto& student : students) {
            for (const auto& subject : student.subjects) {
                allGrades.push_back(subject.grade);

                // Calculate GPA based on the grade and weight
                double gradePoint = subject.grade * subject.weight;
                totalGradePoints += gradePoint;
            }
        }

        if (allGrades.empty()) {
            printw("No grades available!\n");
        } else {
            double minGrade = *std::min_element(allGrades.begin(), allGrades.end());
            double maxGrade = *std::max_element(allGrades.begin(), allGrades.end());
            double averageGrade = std::accumulate(allGrades.begin(), allGrades.end(), 0.0) / allGrades.size();

            double totalWeight = 0.0;
            for (const auto& student : students) {
                for (const auto& subject : student.subjects) {
                    totalWeight += subject.weight;
                }
            }

            double gpa = totalGradePoints / totalWeight;

            printw("Minimum Grade: %.2f\n", minGrade);
            printw("Maximum Grade: %.2f\n", maxGrade);
            printw("Average Grade: %.2f\n", averageGrade);
            printw("GPA: %.2f\n", gpa);
        }
    }

    printw("\n");
    printw("Press any key to continue...\n");
    getch(); // Wait for user input
}


void searchStudent(const std::vector<Student>& students) {
    clear();
    printw("Search Student Information:\n\n");

    printw("Enter student name: ");
    echo();
    char name[256];
    getstr(name);
    noecho();

    bool found = false;
    for (const auto& student : students) {
        if (student.name == name) {
            found = true;

            printw("Name: %s\n", student.name.c_str());
            printw("Age: %d\n", student.age);
            printw("Gender: %s\n", student.gender.c_str());
            printw("Course: %s\n", student.course.c_str());

            if (!student.subjects.empty()) {
                printw("Subjects:\n");
                for (const auto& subject : student.subjects) {
                    printw("Subject: %s, Grade: %.2f, Weight: %.2f\n", subject.name.c_str(), subject.grade, subject.weight);
                }
            }

            printw("\n");
        }
    }

    if (!found) {
        printw("Student not found!\n");
    }

    printw("Press any key to continue...\n");
    getch(); // Wait for user input
}

void displayStudents(const std::vector<Student>& students) {
    clear();
    printw("Student Information:\n\n");

    for (const auto& student : students) {
        printw("Name: %s\n", student.name.c_str());
        printw("Age: %d\n", student.age);
        printw("Gender: %s\n", student.gender.c_str());
        printw("Course: %s\n", student.course.c_str());

        if (!student.subjects.empty()) {
            printw("Subjects:\n");
            for (const auto& subject : student.subjects) {
                printw("Subject: %s, Grade: %.2f, Weight: %.2f\n", subject.name.c_str(), subject.grade, subject.weight);
            }
        }

        printw("\n");
    }

    printw("Press any key to continue...\n");
    getch(); // Wait for user input
}

int main() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Disable echoing of characters

    int choice = 0;
    int highlight = 1; // Highlighted menu option
    bool running = true;
    std::vector<Student> students;

    while (running) {
        clear(); // Clear the screen
        printMenu(); // Display the menu

        int c = getch(); // Get user input

        switch (c) {
            case KEY_UP: // Up arrow key
                if (highlight > 1) {
                    highlight--;
                }
                break;

            case KEY_DOWN: // Down arrow key
                if (highlight < 8) {
                    highlight++;
                }
                break;

            case 10: // Enter key
                choice = highlight;
                break;

            default:
                break;
        }

        // Highlight the selected menu option
        for (int i = 1; i <= 8; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            printw("  "); // Indentation
            switch (i) {
                case 1:
                    printw("1. Add student information\n");
                    break;

                case 2:
                    printw("2. Add subject grade\n");
                    break;

                case 3:
                    printw("3. Save student data to file\n");
                    break;

                case 4:
                    printw("4. Load student data from file\n");
                    break;

                case 5:
                    printw("5. Calculate statistics\n");
                    break;

                case 6:
                    printw("6. Search student information\n");
                    break;

                case 7:
                    printw("7. Display student information\n");
                    break;

                case 8:
                    printw("8. Exit\n");
                    break;
            }
            attroff(A_REVERSE);
        }

        refresh(); // Update the screen

        if (choice != 0) {
            switch (choice) {
                case 1:
                    addStudentInformation(students);
                    break;

                case 2:
                    addSubjectGrade(students);
                    break;

                case 3:
                    saveStudentDataToFile(students);
                    break;

                case 4:
                    loadStudentDataFromFile(students);
                    break;

                case 5:
                    calculateStatistics(students);
                    break;

                case 6:
                    searchStudent(students);
                    break;

                case 7:
                    displayStudents(students);
                    break;

                case 8:
                    running = false;
                    break;

                default:
                    printw("Invalid choice! Please try again.\n");
                    break;
            }

            choice = 0; // Reset the choice
        }
    }

    endwin(); // Clean up ncurses

    return 0;
}
