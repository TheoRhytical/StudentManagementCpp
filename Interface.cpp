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
    printw("6. Search student information\n");
    printw("7. Display student information\n");
    printw("8. Exit\n\n");
    printw("Enter your choice: ");
}

void addStudentInformation(std::vector<Student>& students) {
    clear();
    printw("Add Student Information:\n\n");

    Student student;

    echo();
    printw("Enter student name: ");
    char name[256];
    getstr(name);
    student.name = name;

    int age;
    echo();
    printw("Enter student age: ");
    scanw("%d", &age);
    student.age = age;

    echo();
    printw("Enter student gender: ");
    char gender[256];
    getstr(gender);
    student.gender = gender;

    echo();
    printw("Enter student course: ");
    char course[256];
    getstr(course);
    student.course = course;

    students.push_back(student);

    printw("\nStudent information added successfully!\n");
    getch(); // Wait for user input
}

void addSubjectGrade(std::vector<Student>& students) {
    clear();
    printw("Add Subject Grade:\n\n");

    echo();
    printw("Enter student name: ");
    char name[256];
    getstr(name);
    noecho();

    bool found = false;
    for (auto& student : students) {
        if (student.name == name) {
            found = true;

            char subject[256];
            echo();
            printw("Enter subject name: ");
            getstr(subject);

            double grade;
            echo();
            printw("Enter subject grade: ");
            scanw("%lf", &grade);

            double weight;
            echo();
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

    echo();
    printw("Enter the file name: ");
    char input[256];
    getstr(input);
    noecho();
    std::string filename = input;

    std::ofstream file(filename);

    if (!file.is_open()) {
        printw("\nFailed to open the file!\n");
    } else {
        for (const auto& student : students) {
            file << "Name: " << student.name << "\n";
            file << "Age: " << student.age << "\n";
            file << "Gender: " << student.gender << "\n";
            file << "Course: " << student.course << "\n";
            file << "Subjects:\n";

            for (const auto& subject : student.subjects) {
                file << "- Name: " << subject.name << "\n";
                file << "  Grade: " << subject.grade << "\n";
                file << "  Weight: " << subject.weight << "\n";
            }

            file << "\n";
        }

        printw("\nStudent data saved to file successfully!\n");
    }

    getch(); // Wait for user input
}

void loadStudentDataFromFile(std::vector<Student>& students) {
    clear();
    printw("Load Student Data from File:\n\n");

    echo();
    printw("Enter the file name: ");
    char input[256];
    getstr(input);
    noecho();
    std::string filename = input;

    std::ifstream file(filename);

    if (!file.is_open()) {
        printw("\nFailed to open the file!\n");
    } else {
        students.clear();
        std::string line;

        Student student;
        bool readingSubjects = false;

        while (std::getline(file, line)) {
            if (line.empty()) {
                students.push_back(student);
                student = Student();
                readingSubjects = false;
            } else if (line.find("Name: ") == 0) {
                student.name = line.substr(6);
            } else if (line.find("Age: ") == 0) {
                student.age = std::stoi(line.substr(5));
            } else if (line.find("Gender: ") == 0) {
                student.gender = line.substr(8);
            } else if (line.find("Course: ") == 0) {
                student.course = line.substr(8);
            } else if (line.find("Subjects:") == 0) {
                readingSubjects = true;
            } else if (readingSubjects && line.find("- Name: ") == 0) {
                Subject subject;
                subject.name = line.substr(8);
                std::getline(file, line); // Read the grade line
                subject.grade = std::stod(line.substr(9));
                std::getline(file, line); // Read the weight line
                subject.weight = std::stod(line.substr(10));

                student.subjects.push_back(subject);
            }
        }

        printw("\nStudent data loaded from file successfully!\n");
    }

    getch(); // Wait for user input
}

void calculateStatistics(const std::vector<Student>& students) {
    clear();
    printw("Grade Statistics:\n\n");

    if (students.empty()) {
        printw("No students available!\n");
    } else {
        echo();
        printw("Enter student name: ");
        char name[256];
        getstr(name);
        noecho();

        bool found = false;
        for (const auto& student : students) {
            if (student.name == name) {
                found = true;
                std::vector<double> allGrades;
                double totalGradePoints = 0.0;
                double totalWeight = 0.0;

                for (const auto& subject : student.subjects) {
                    allGrades.push_back(subject.grade);

                    // Calculate GWA based on the grade and weight
                    double gwa;
                    if (subject.grade >= 93 && subject.grade <= 100) {
                        gwa = 1.00;
                    } else if (subject.grade >= 90 && subject.grade < 93) {
                        gwa = 1.25;
                    } else if (subject.grade >= 87 && subject.grade < 90) {
                        gwa = 1.50;
                    } else if (subject.grade >= 84 && subject.grade < 87) {
                        gwa = 1.75;
                    } else if (subject.grade >= 80 && subject.grade < 84) {
                        gwa = 2.00;
                    } else if (subject.grade >= 75 && subject.grade < 80) {
                        gwa = 2.25;
                    } else if (subject.grade >= 70 && subject.grade < 75) {
                        gwa = 2.50;
                    } else if (subject.grade >= 65 && subject.grade < 70) {
                        gwa = 2.75;
                    } else if (subject.grade >= 60 && subject.grade < 65) {
                        gwa = 3.00;
                    } else if (subject.grade >= 55 && subject.grade < 60) {
                        gwa = 4.00;
                    } else {
                        gwa = 5.00;
                    }

                    double gradePoint = gwa * subject.weight;
                    totalGradePoints += gradePoint;
                    totalWeight += subject.weight;
                }

                if (allGrades.empty()) {
                    printw("No grades available for %s!\n", student.name.c_str());
                } else {
                    double minGrade = *std::min_element(allGrades.begin(), allGrades.end());
                    double maxGrade = *std::max_element(allGrades.begin(), allGrades.end());
                    double averageGrade = std::accumulate(allGrades.begin(), allGrades.end(), 0.0) / allGrades.size();

                    double gwa = totalGradePoints / totalWeight;

                    printw("Student Name: %s\n", student.name.c_str());
                    printw("Minimum Grade: %.2f\n", minGrade);
                    printw("Maximum Grade: %.2f\n", maxGrade);
                    printw("Average Grade: %.2f\n", averageGrade);
                    printw("GWA: %.2f\n", gwa);
                }

                break;
            }
        }

        if (!found) {
            printw("\nStudent not found!\n");
        }

        // Calculate overall statistics for all students
        std::vector<double> allGrades;
        double totalGradePoints = 0.0;
        double totalWeight = 0.0;

        for (const auto& student : students) {
            for (const auto& subject : student.subjects) {
                allGrades.push_back(subject.grade);

                double gwa;
                if (subject.grade >= 93 && subject.grade <= 100) {
                    gwa = 1.00;
                } else if (subject.grade >= 90 && subject.grade < 93) {
                    gwa = 1.25;
                } else if (subject.grade >= 87 && subject.grade < 90) {
                    gwa = 1.50;
                } else if (subject.grade >= 84 && subject.grade < 87) {
                    gwa = 1.75;
                } else if (subject.grade >= 80 && subject.grade < 84) {
                    gwa = 2.00;
                } else if (subject.grade >= 75 && subject.grade < 80) {
                    gwa = 2.25;
                } else if (subject.grade >= 70 && subject.grade < 75) {
                    gwa = 2.50;
                } else if (subject.grade >= 65 && subject.grade < 70) {
                    gwa = 2.75;
                } else if (subject.grade >= 60 && subject.grade < 65) {
                    gwa = 3.00;
                } else if (subject.grade >= 55 && subject.grade < 60) {
                    gwa = 4.00;
                } else {
                    gwa = 5.00;
                }

                double gradePoint = gwa * subject.weight;
                totalGradePoints += gradePoint;
                totalWeight += subject.weight;
            }
        }

        if (!allGrades.empty()) {
            double minGrade = *std::min_element(allGrades.begin(), allGrades.end());
            double maxGrade = *std::max_element(allGrades.begin(), allGrades.end());
            double averageGrade = std::accumulate(allGrades.begin(), allGrades.end(), 0.0) / allGrades.size();

            double gwa = totalGradePoints / totalWeight;

            printw("\nOverall Student Statistics:\n");
            printw("Minimum Grade: %.2f\n", minGrade);
            printw("Maximum Grade: %.2f\n", maxGrade);
            printw("Average Grade: %.2f\n", averageGrade);
            printw("Overall GWA: %.2f\n", gwa);
        }
    }

    printw("\n");
    printw("Press any key to continue...\n");
    getch(); // Wait for user input
}


void searchStudentInformation(const std::vector<Student>& students) {
    clear();
    printw("Search Student Information:\n\n");

    echo();
    printw("Enter student name: ");
    char name[256];
    getstr(name);
    noecho();

    bool found = false;
    for (const auto& student : students) {
        if (student.name == name) {
            found = true;
            printw("\nStudent Information:\n");
            printw("Name: %s\n", student.name.c_str());
            printw("Age: %d\n", student.age);
            printw("Gender: %s\n", student.gender.c_str());
            printw("Course: %s\n", student.course.c_str());
            printw("Subjects:\n");

            for (const auto& subject : student.subjects) {
                printw("- Name: %s\n", subject.name.c_str());
                printw("  Grade: %.2lf\n", subject.grade);
                printw("  Weight: %.2lf\n", subject.weight);
            }

            break;
        }
    }

    if (!found) {
        printw("\nStudent not found!\n");
    }

    getch(); // Wait for user input
}

void displayStudentInformation(const std::vector<Student>& students) {
    clear();
    printw("Display Student Information:\n\n");

    if (students.empty()) {
        printw("No student data available!\n");
    } else {
        for (const auto& student : students) {
            printw("Name: %s\n", student.name.c_str());
            printw("Age: %d\n", student.age);
            printw("Gender: %s\n", student.gender.c_str());
            printw("Course: %s\n", student.course.c_str());
            printw("Subjects:\n");

            for (const auto& subject : student.subjects) {
                printw("- Name: %s\n", subject.name.c_str());
                printw("  Grade: %.2lf\n", subject.grade);
                printw("  Weight: %.2lf\n", subject.weight);
            }

            printw("\n");
        }
    }

    getch(); // Wait for user input
}

int main() {
    initscr(); // Initialize the screen
    keypad(stdscr, true); // Enable keyboard input
    noecho(); // Disable automatic echoing of typed characters
    curs_set(0); // Hide the cursor

    std::vector<Student> students;

    std::string choiceStr;
    while (true) {
        clear();
        printMenu();
        echo();
        char input[256];
        getstr(input);
        noecho();
        choiceStr = input;

    int choice;
    try {
        choice = std::stoi(choiceStr);
        // Handle the choice...
    } catch (const std::exception&) {
        printw("Invalid choice! Please try again.\n");
        getch(); // Wait for user input
        continue;
    }

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
                searchStudentInformation(students);
                break;
            case 7:
                displayStudentInformation(students);
                break;
            case 8:
                endwin(); // End the window
                return 0;
            default:
                printw("Invalid choice! Please try again.\n");
                getch(); // Wait for user input
                break;
        }
    }
}
