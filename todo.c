#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 10
#define FILENAME "tasks.dat"

typedef struct {
    char task[100];
    int completed;
} Task;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

void saveTasksToFile(Task tasks[], int taskCount) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(tasks, sizeof(Task), taskCount, file);

    fclose(file);
}

void loadTasksFromFile(Task tasks[], int *taskCount) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No previous tasks found.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    *taskCount = fileSize / sizeof(Task);
    if (*taskCount > MAX_TASKS) {
        printf("Warning: Maximum task limit exceeded. Loading first %d tasks.\n", MAX_TASKS);
        *taskCount = MAX_TASKS;
    }

    fread(tasks, sizeof(Task), *taskCount, file);

    fclose(file);
}

void displayMenu() {
    printf("\nTODO App\n");
    printf("1. Add Task || ");
    printf("2. View Tasks || ");
    printf("3. Mark Task as Completed || ");
    printf("4. Exit || \n");
}

void addTask(Task tasks[], int *taskCount) {
    clearScreen();
    if (*taskCount < MAX_TASKS) {
        printf("Enter the task: ");
        scanf(" %[^\n]", tasks[*taskCount].task);
        tasks[*taskCount].completed = 0; // Initialize as not completed
        (*taskCount)++;
        saveTasksToFile(tasks, *taskCount); // Save tasks after adding
        printf("Task added successfully!\n");
    } else {
        printf("Task limit reached. Cannot add more tasks.\n");
    }
}

void viewTasks(Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available.\n");
    } else {
        printf("\nTasks:\n");
        for (int i = 0; i < taskCount; i++) {
            printf("%d. %s - %s\n", i + 1, tasks[i].completed ? "[X]" : "[ ]", tasks[i].task);
        }
    }
  
}

void markCompleted(Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available to mark as completed.\n");
        return;
    }

    printf("Enter the task number to mark as completed: ");
    int taskNumber;
    scanf("%d", &taskNumber);

    if (taskNumber >= 1 && taskNumber <= taskCount) {
        tasks[taskNumber - 1].completed = 1;
        saveTasksToFile(tasks, taskCount); // Save tasks after marking as completed
        printf("Task marked as completed!\n");
    } else {
        printf("Invalid task number.\n");
    }
}

int main() {
    Task tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    loadTasksFromFile(tasks, &taskCount); // Load tasks at the start

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(tasks, &taskCount);
                break;
            case 2:
                viewTasks(tasks, taskCount);
                break;
            case 3:
                markCompleted(tasks, taskCount);
                break;
            case 4:
                printf("Exiting ToDo App. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
