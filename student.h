#ifndef STUDENT_H
#define STUDENT_H

#define STUDENT_NAME_LENGTH 50
#define STUDENT_DATA_FILE "students.dat"

typedef struct {
    int id;
    char name[STUDENT_NAME_LENGTH];
    float marks;
} Student;

/* Shared interface: other members should use this exact structure and prototypes. */
int student_id_exists(int id);
int add_student_record(const Student *student);
void add_student_interactive(void);
void display_students(void);

#endif
