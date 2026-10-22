
#include <stdio.h>
#include <time.h>
#include "student.h"
#include "sequential_search.h"

int sequential_search(int target_id,
                     Student *result,
                     int *records_examined,
                     double *elapsed_seconds)
{
    FILE *fp;
    Student current;
    clock_t start, end;

    if (result == NULL ||
        records_examined == NULL ||
        elapsed_seconds == NULL)
    {
        return -1;
    }

    *records_examined = 0;
    *elapsed_seconds = 0.0;

    fp = fopen(STUDENT_DATA_FILE, "rb");

    if (fp == NULL)
    {
        return -1;
    }

    start = clock();

    while (fread(&current, sizeof(Student), 1, fp) == 1)
    {
        (*records_examined)++;

        if (current.id == target_id)
        {
            *result = current;

            end = clock();
            *elapsed_seconds =
                (double)(end - start) / CLOCKS_PER_SEC;

            if (fclose(fp) != 0)
                return -1;

            return 1;
        }
    }

    end = clock();
    *elapsed_seconds =
        (double)(end - start) / CLOCKS_PER_SEC;

    if (ferror(fp))
    {
        fclose(fp);
        return -1;
    }

    if (fclose(fp) != 0)
        return -1;

    return 0;
}
