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

void printMenu(int highlight) {
    clear();
    start_color();

    // Define color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // White text on black background
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Green text on black background
    init_pair(3, COLOR_CYAN, COLOR_BLACK);   // Cyan text on black background
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Red text on black background

    attron(COLOR_PAIR(1) | A_BOLD); // Enable color and bold text
    printw("**************************************************\n");
    printw("*          Student Grade Management System       *\n");
    printw("**************************************************\n\n");
    attroff(COLOR_PAIR(1) | A_BOLD); // Disable color and bold text
    printw("Menu:\n\n");
    printw("%s1. Add Student Information\n\n", (highlight == 0) ? ">> " : "  ");
    printw("%s2. Add Subject Grade\n\n", (highlight == 1) ? ">> " : "  ");
    printw("%s3. Save Student Data to File\n\n", (highlight == 2) ? ">> " : "  ");
    printw("%s4. Load Student Data from File\n\n", (highlight == 3) ? ">> " : "  ");
    printw("%s5. Calculate Statistics\n\n", (highlight == 4) ? ">> " : "  ");
    printw("%s6. Search Student Information\n\n", (highlight == 5) ? ">> " : "  ");
    printw("%s7. Display Student Information\n\n", (highlight == 6) ? ">> " : "  ");
    printw("%s8. Exit\n\n", (highlight == 7) ? ">> " : "  ");
    printw("\n");
    attron(COLOR_PAIR(1) | A_BOLD); // Enable bold text
    printw("**************************************************\n");
    attroff(COLOR_PAIR(1) | A_BOLD); // Disable bold text
    refresh();
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
    initscr();            // Initialize ncurses
    clear();              // Clear the screen
    noecho();             // Turn off echoing of characters
    cbreak();             // Disable line buffering, receive input character by character
    keypad(stdscr, TRUE); // Enable special keys, such as arrow keys

    int highlight = 0;
    int choice;
    bool running = true;
    std::vector<Student> students;

    while (running) {
        printMenu(highlight);
        choice = getch(); // Wait for user input

        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? 7 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == 7) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                switch (highlight) {
                    case 0:
                        addStudentInformation(students);
                        break;
                    case 1:

                        addSubjectGrade(students);
                        break;
                    case 2:
                        saveStudentDataToFile(students);
                        break;
                    case 3:
                        loadStudentDataFromFile(students);
                        break;
                    case 4:
                        calculateStatistics(students);
                        break;
                    case 5:
                        searchStudentInformation(students);
                        break;
                    case 6:
                        displayStudentInformation(students);
                        break;
                    case 7:
                        running = false;
                        break;
                }
                break;
        }
    }

    endwin(); // Clean up and restore the terminal settings
    return 0;
}
