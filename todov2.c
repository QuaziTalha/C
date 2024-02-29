#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MAX_TASKS 10
char FILENAME[256];

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
    printf("1. Add Task || ");\
    printf("2. Edit Task || ");
    printf("3. Mark Task as Completed || ");
    printf("4. Delete Task || \n");
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

void addTaskFromString(Task tasks[], int *taskCount, const char *taskDescription) {
    clearScreen();
    if (*taskCount < MAX_TASKS) {
        snprintf(tasks[*taskCount].task, sizeof(tasks[*taskCount].task), "%s", taskDescription);
        tasks[*taskCount].completed = 0; // Initialize as not completed
        (*taskCount)++;
        saveTasksToFile(tasks, *taskCount); // Save tasks after adding
        printf("Task added successfully!\n");
    } else {
        printf("Task limit reached. Cannot add more tasks.\n");
    }
}

void checkTask(Task tasks[], int taskCount, int taskNumber) {
    if (taskNumber >= 1 && taskNumber <= taskCount) {
        tasks[taskNumber - 1].completed = 1;
        saveTasksToFile(tasks, taskCount); 
        printf("Task marked as completed!\n");
    } else {
        printf("Invalid task number.\n");
    }
}

void editTask(Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available to edit.\n");
        return;
    }

    printf("Enter the task number to edit: ");
    int taskNumber;
    scanf("%d", &taskNumber);

    if (taskNumber >= 1 && taskNumber <= taskCount) {
        printf("Enter the new task: ");
        scanf(" %[^\n]", tasks[taskNumber - 1].task);
        saveTasksToFile(tasks, taskCount); // Save tasks after editing
        printf("Task edited successfully!\n");
    } else {
        printf("Invalid task number.\n");
    }
}

void viewTasks(Task tasks[], int taskCount) {
    clearScreen();

    if (taskCount == 0) {
        printf("No tasks available.\n");
    } else {
        printf("\nTasks:\n");
        for (int i = 0; i < taskCount; i++) {
            printf("%d. %s - %s\n", i + 1, tasks[i].completed ? "[X]" : "[ ]", tasks[i].task);
        }
    }

    // Display the menu at the bottom
    displayMenu();
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

void deleteTask(Task tasks[], int *taskCount) {
    if (*taskCount == 0) {
        printf("No tasks available to delete.\n");
        return;
    }

    printf("Enter the task number to delete: ");
    int taskNumber;
    scanf("%d", &taskNumber);

    if (taskNumber >= 1 && taskNumber <= *taskCount) {
        // Move tasks after the deleted task to fill the gap
        for (int i = taskNumber - 1; i < *taskCount - 1; i++) {
            tasks[i] = tasks[i + 1];
        }

        (*taskCount)--;
        saveTasksToFile(tasks, *taskCount); // Save tasks after deletion
        printf("Task deleted successfully!\n");
    } else {
        printf("Invalid task number.\n");
    }
}

int main() {
    snprintf(FILENAME, sizeof(FILENAME), "C:\\Users\\%s\\tasks.dat", getenv("USERNAME"));

    Task tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    loadTasksFromFile(tasks, &taskCount); 

    do {
        viewTasks(tasks, taskCount);
        printf("Enter your choice: ");
        char input[100];
        scanf(" %[^\n]", input);

        if (strncmp(input, "add", 3) == 0) {
            addTaskFromString(tasks, &taskCount, input + 4); 
        } else if (strncmp(input, "check", 5) == 0) {
            int taskNumber;
            if (sscanf(input + 6, "%d", &taskNumber) == 1) {
                checkTask(tasks, taskCount, taskNumber);
            } else {
                printf("Invalid command format. Use 'check [number]'.\n");
            }
        } else {
            sscanf(input, "%d", &choice);

            switch (choice) {
                case 1:
                    addTask(tasks, &taskCount);
                    break;
                case 2:
                    markCompleted(tasks, taskCount);
                    break;
                case 3:
                    editTask(tasks, taskCount);
                    break;
                case 4:
                    deleteTask(tasks, &taskCount);
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } while (choice != 5);

    return 0;
}
