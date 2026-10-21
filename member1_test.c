#include "student.h"

#include <stdio.h>
#include <stdlib.h>

static int read_choice(int *choice)
{
    char line[40];
    char extra;
    int value;

    printf("\nMember 1 - File Handling Test\n");
    printf("1. Add student record\n");
    printf("2. Display saved records\n");
    printf("3. Check whether an ID exists\n");
    printf("0. Exit\n");
    printf("Choice: ");

    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    if (sscanf(line, " %d %c", &value, &extra) != 1) {
        printf("Invalid choice. Enter a menu number.\n");
        return 1;
    }

    *choice = value;
    return 1;
}

int main(void)
{
    int choice;

    while (read_choice(&choice)) {
        if (choice == 0) {
            printf("Exiting member 1 test.\n");
            break;
        } else if (choice == 1) {
            add_student_interactive();
        } else if (choice == 2) {
            display_students();
        } else if (choice == 3) {
            char line[40];
            char extra;
            int id;
            int result;

            printf("Enter student ID to check: ");
            if (fgets(line, sizeof line, stdin) == NULL) {
                printf("\nInput ended.\n");
                break;
            }

            if (sscanf(line, " %d %c", &id, &extra) != 1 || id <= 0) {
                printf("Invalid ID.\n");
                continue;
            }

            result = student_id_exists(id);
            if (result == 1) {
                printf("ID %d exists.\n", id);
            } else if (result == 0) {
                printf("ID %d was not found.\n", id);
            } else {
                printf("Could not complete the ID check due to a file error.\n");
            }
        } else {
            printf("Invalid choice. Choose 0, 1, 2, or 3.\n");
        }
    }

    return 0;
}
