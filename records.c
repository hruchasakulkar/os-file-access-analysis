
#include "student.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

/* Read a line and reject input that is too long. */
static int read_line(const char *prompt, char *text, size_t size)
{
    int ch;
    size_t length;

    printf("%s", prompt);

    if (fgets(text, (int)size, stdin) == NULL)
        return 0;

    length = strlen(text);

    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
        return 1;
    }

    ch = getchar();

    if (ch == '\n' || ch == EOF)
        return 1;

    while (ch != '\n' && ch != EOF)
        ch = getchar();

    printf("Input is too long. Please try again.\n");
    return -1;
}

/* Read and validate the student ID. */
static int read_id(int *id)
{
    char text[100];
    char extra;
    long value;
    int result;

    result = read_line("Enter positive student ID: ",
                       text, sizeof(text));

    if (result != 1)
        return result;

    if (sscanf(text, "%ld %c", &value, &extra) != 1 ||
        value < 1 || value > INT_MAX) {
        printf("Invalid ID. Enter a positive whole number.\n");
        return -1;
    }

    *id = (int)value;
    return 1;
}

/* Read and validate student marks. */
static int read_marks(float *marks)
{
    char text[100];
    char extra;
    float value;
    int result;

    result = read_line("Enter marks (0-100): ",
                       text, sizeof(text));

    if (result != 1)
        return result;

    if (sscanf(text, "%f %c", &value, &extra) != 1 ||
        !isfinite(value) || value < 0 || value > 100) {
        printf("Invalid marks. Enter a number from 0 to 100.\n");
        return -1;
    }

    *marks = value;
    return 1;
}

/* Return 1 if found, 0 if absent, and -1 if an error occurs. */
int student_id_exists(int id)
{
    FILE *file;
    Student student;
    long file_size;

    file = fopen(STUDENT_DATA_FILE, "rb");

    if (file == NULL) {
        if (errno == ENOENT)
            return 0;

        perror("Could not open student data file");
        return -1;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            fclose(file);
            return 1;
        }
    }

    if (ferror(file)) {
        perror("Error reading student data file");
        fclose(file);
        return -1;
    }

    /* Detect a file whose size is not a whole number of records. */
    if (fseek(file, 0, SEEK_END) == 0) {
        file_size = ftell(file);

        if (file_size >= 0 &&
            file_size % (long)sizeof(Student) != 0) {
            printf("Warning: incomplete record in data file.\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

/* Validate and save one student record in binary format. */
int add_student_record(const Student *student)
{
    FILE *file;
    int result;

    if (student == NULL || student->id <= 0 ||
        student->name[0] == '\0' ||
        strspn(student->name, " \t\r\n") ==
            strlen(student->name) ||
        !isfinite(student->marks) ||
        student->marks < 0 || student->marks > 100) {
        printf("Cannot save invalid student data.\n");
        return 0;
    }

    /* Ensure the name ends inside its character array. */
    if (memchr(student->name, '\0',
               STUDENT_NAME_LENGTH) == NULL) {
        printf("Student name is too long.\n");
        return 0;
    }

    result = student_id_exists(student->id);

    if (result == 1) {
        printf("Student ID %d already exists.\n", student->id);
        return 0;
    }

    if (result == -1) {
        printf("Could not safely check the student ID.\n");
        return 0;
    }

    file = fopen(STUDENT_DATA_FILE, "ab");

    if (file == NULL) {
        perror("Could not open file for writing");
        return 0;
    }

    if (fwrite(student, sizeof(Student), 1, file) != 1) {
        perror("Could not save student record");
        fclose(file);
        return 0;
    }

    if (fclose(file) != 0) {
        perror("Could not close student data file");
        return 0;
    }

    printf("Student record saved successfully.\n");
    return 1;
}

/* Collect student details from the user. */
void add_student_interactive(void)
{
    Student student;
    char name[STUDENT_NAME_LENGTH];
    char *start;
    size_t length;
    int result;

    result = read_id(&student.id);

    if (result != 1) {
        if (result == 0)
            printf("\nInput ended.\n");
        return;
    }

    result = read_line("Enter student name: ",
                       name, sizeof(name));

    if (result != 1) {
        if (result == 0)
            printf("\nInput ended.\n");
        return;
    }

    /* Remove spaces from the beginning and end of the name. */
    start = name;

    while (*start == ' ' || *start == '\t')
        start++;

    length = strlen(start);

    while (length > 0 &&
           (start[length - 1] == ' ' ||
            start[length - 1] == '\t' ||
            start[length - 1] == '\r')) {
        start[--length] = '\0';
    }

    if (length == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    memmove(student.name, start, length + 1);

    result = read_marks(&student.marks);

    if (result != 1) {
        if (result == 0)
            printf("\nInput ended.\n");
        return;
    }

    add_student_record(&student);
}

/* Display all complete student records. */
void display_students(void)
{
    FILE *file;
    Student student;
    int count = 0;
    long file_size;

    file = fopen(STUDENT_DATA_FILE, "rb");

    if (file == NULL) {
        if (errno == ENOENT)
            printf("No student records found.\n");
        else
            perror("Could not open student data file");

        return;
    }

    printf("\n%-10s %-30s %-10s\n",
           "ID", "NAME", "MARKS");
    printf("------------------------------------------------------\n");

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        printf("%-10d %-30s %-10.2f\n",
               student.id, student.name, student.marks);
        count++;
    }

    if (ferror(file)) {
        perror("Error reading student data file");
    } else if (fseek(file, 0, SEEK_END) == 0) {
        file_size = ftell(file);

        if (file_size >= 0 &&
            file_size % (long)sizeof(Student) != 0) {
            printf("Warning: file contains an incomplete record.\n");
        }
    }

    printf("------------------------------------------------------\n");
    printf("Total complete records: %d\n\n", count);

    if (fclose(file) != 0)
        perror("Could not close student data file");
}
