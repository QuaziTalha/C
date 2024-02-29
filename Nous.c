#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>  // Add this line for tolower function

// Function prototype for showCurrentTime
void showCurrentTime();

void greetUser() {
    printf("NOUS: Hello! I am NOUS, a rudimentary AI designed to be a Databank for Qu Taha and perform various tasks.\n");
}

int containsKeyword(const char *input, const char *keyword) {
    // Check if the keyword is present in the input string
    return strstr(input, keyword) != NULL;
}

void processUserInput(const char *input) {
    if (containsKeyword(input, "hello")) {
        printf("NOUS: Hi there!\n");
    } else if (containsKeyword(input, "how are you")) {
        printf("NOUS: I'm just a computer program, but I'm doing well. How can I assist you today?\n");
    } else if (containsKeyword(input, "time")) {
        showCurrentTime();
    } else if (containsKeyword(input, "bye")) {
        printf("NOUS: Goodbye! If you have any more questions or tasks, feel free to ask.\n");
    } else {
        printf("NOUS: I dont quite understand.\n");
    }
}

void showCurrentTime() {
    time_t currentTime;
    struct tm *localTime;

    // Get current time
    time(&currentTime);

    // Convert to local time
    localTime = localtime(&currentTime);

    // Print the time
    printf("NOUS: The current time is: %02d:%02d:%02d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}

int main() {
    char userInput[100];

    greetUser();

    while (1) {
        printf("You: ");
        fgets(userInput, sizeof(userInput), stdin);

        // Remove newline character from user input
        userInput[strcspn(userInput, "\n")] = 0;

        // Convert input to lowercase for case-insensitive comparison
        for (int i = 0; userInput[i]; i++) {
            userInput[i] = tolower(userInput[i]);
        }

        if (containsKeyword(userInput, "exit")) {
            printf("NOUS: Goodbye! If you need assistance in the future, don't hesitate to reach out.\n");
            break;
        }

        processUserInput(userInput);
    }

    return 0;
}
