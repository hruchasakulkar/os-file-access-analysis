MEMBER 1 - FILE HANDLING AND STUDENT RECORDS

Files:
- student.h       Shared Student structure and function prototypes
- records.c       Add/display/search-for-duplicate functions and file handling
- member1_test.c  Small standalone test menu (not the final group main menu)

Compile on macOS/Linux:
    gcc -std=c11 -Wall -Wextra -pedantic records.c member1_test.c -o member1_test

Run:
    ./member1_test

The program creates students.dat in the current working directory.
It uses binary records and append mode so previously saved records are retained.
Do not edit students.dat manually. Keep it in the same folder while testing.

Important shared interfaces:
    int student_id_exists(int id);
        Returns 1 if found, 0 if not found, -1 on a read error.

    int add_student_record(const Student *student);
        Returns 1 on successful save, 0 if not saved.

    void add_student_interactive(void);
    void display_students(void);

Before integration:
- Send student.h to all members and agree to use this exact Student structure.
- Member 2 can read Student records with fread().
- Member 3 can use ftell() to capture each record's file position.
- Member 4 should own the final main menu; member1_test.c is only a standalone test.

Test checklist (record actual results; do not assume success):
1. Display records before adding any.
2. Add two different IDs and display both.
3. Exit and rerun; verify records persist.
4. Try the same ID again; it should be rejected.
5. Try marks below 0 and above 100.
6. Try a blank name.
7. Search for an existing ID and a missing ID.
8. If the file cannot be opened, check the error message.
