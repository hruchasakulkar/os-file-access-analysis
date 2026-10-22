
#ifndef INDEXED_SEARCH_H
#define INDEXED_SEARCH_H

#include <stddef.h>
#include "student.h"

typedef struct {
    size_t entry_count;
    size_t index_bytes;
    size_t memory_budget_bytes;
} IndexStats;

int build_student_index(size_t memory_budget_bytes);

int indexed_search(int id, Student *result,
                   size_t *comparisons);

void get_index_stats(IndexStats *stats);

void invalidate_student_index(void);

#endif
