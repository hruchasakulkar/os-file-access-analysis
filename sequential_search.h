#ifndef SEQUENTIAL_SEARCH_H
#define SEQUENTIAL_SEARCH_H

#include "student.h"

int sequential_search(
    int target_id,         
    Student *result,
    int *records_examined,
    double *elapsed_seconds);

#endif