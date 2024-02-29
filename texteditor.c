#include <stdio.h>
#include <stdlib.h>
#include <pdcurses/curses.h>

void initEditor() {
    initscr(); // Initialize the curses library
    raw();     // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys
    noecho(); // Don't display typed characters
}

void closeEditor() {
    endwin(); // End curses mode
}

void displayFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printw("Error opening file.\n");
        return;
    }

    clear(); // Clear the screen

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        printw("%c", ch); // Print character to the screen
    }

    fclose(file);
    refresh(); // Refresh the screen
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    initEditor(); // Initialize the text editor

    displayFile(filename); // Display the contents of the file

    printw("\nPress any key to exit...");
    refresh();

    getch(); // Wait for a key press
    closeEditor(); // Close the text editor

    return 0;
}
