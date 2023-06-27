#include <ncurses.h>
#include <string>

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
                // Logic to add student information
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
