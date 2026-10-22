
#include "indexed_search.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int id;
    long file_position;
} IndexEntry;

static IndexEntry *student_index = NULL;
static size_t student_index_count = 0;
static size_t student_index_bytes = 0;
static size_t student_memory_budget = 0;
static int index_is_ready = 0;

static int compare_index_entries(const void *a, const void *b)
{
    const IndexEntry *x = a;
    const IndexEntry *y = b;

    if (x->id < y->id) return -1;
    if (x->id > y->id) return 1;
    return 0;
}

void invalidate_student_index(void)
{
    free(student_index);
    student_index = NULL;
    student_index_count = 0;
    student_index_bytes = 0;
    student_memory_budget = 0;
    index_is_ready = 0;
}

int build_student_index(size_t memory_budget_bytes)
{
    FILE *file;
    Student student;
    long file_size;
    size_t count, required_bytes, i;
    IndexEntry *new_index = NULL;

    invalidate_student_index();

    file = fopen(STUDENT_DATA_FILE, "rb");
    if (file == NULL) {
        perror("Could not open students.dat");
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Could not seek data file");
        fclose(file);
        return -1;
    }

    file_size = ftell(file);
    if (file_size < 0) {
        perror("Could not determine file size");
        fclose(file);
        return -1;
    }

    if (file_size % (long)sizeof(Student) != 0) {
        fprintf(stderr, "Data file contains an incomplete record.\n");
        fclose(file);
        return -1;
    }

    count = (size_t)(file_size / (long)sizeof(Student));

    if (count > SIZE_MAX / sizeof(IndexEntry)) {
        fprintf(stderr, "Index size overflow.\n");
        fclose(file);
        return -1;
    }

    required_bytes = count * sizeof(IndexEntry);

    printf("Records: %zu\n", count);
    printf("Index memory required: %zu bytes\n", required_bytes);
    printf("Memory budget: %zu bytes\n", memory_budget_bytes);

    if (required_bytes > memory_budget_bytes) {
        printf("Insufficient memory budget; index not built.\n");
        fclose(file);
        return 0;
    }

    if (count > 0) {
        new_index = malloc(required_bytes);
        if (new_index == NULL) {
            perror("Could not allocate index");
            fclose(file);
            return -1;
        }
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("Could not rewind data file");
        free(new_index);
        fclose(file);
        return -1;
    }

    for (i = 0; i < count; i++) {
        long position = ftell(file);

        if (position < 0 ||
            fread(&student, sizeof(Student), 1, file) != 1) {
            fprintf(stderr, "Error reading student record.\n");
            free(new_index);
            fclose(file);
            return -1;
        }

        new_index[i].id = student.id;
        new_index[i].file_position = position;
    }

    if (fclose(file) != 0) {
        perror("Could not close data file");
        free(new_index);
        return -1;
    }

    qsort(new_index, count, sizeof(IndexEntry),
          compare_index_entries);

    student_index = new_index;
    student_index_count = count;
    student_index_bytes = required_bytes;
    student_memory_budget = memory_budget_bytes;
    index_is_ready = 1;

    printf("Index built successfully.\n");
    return 1;
}

int indexed_search(int id, Student *result, size_t *comparisons)
{
    size_t low = 0;
    size_t high = student_index_count;
    size_t checks = 0;
    size_t position = 0;
    int found = 0;
    FILE *file;

    if (comparisons != NULL)
        *comparisons = 0;

    if (!index_is_ready || result == NULL || id <= 0)
        return -1;

    while (low < high) {
        size_t mid = low + (high - low) / 2;
        checks++;

        if (student_index[mid].id == id) {
            position = mid;
            found = 1;
            break;
        }

        if (student_index[mid].id < id)
            low = mid + 1;
        else
            high = mid;
    }

    if (comparisons != NULL)
        *comparisons = checks;

    if (!found)
        return 0;

    file = fopen(STUDENT_DATA_FILE, "rb");
    if (file == NULL) {
        perror("Could not open students.dat");
        return -1;
    }

    if (fseek(file, student_index[position].file_position,
              SEEK_SET) != 0) {
        perror("Could not seek to indexed record");
        fclose(file);
        return -1;
    }

    if (fread(result, sizeof(Student), 1, file) != 1) {
        fprintf(stderr, "Could not read indexed record.\n");
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0) {
        perror("Could not close data file");
        return -1;
    }

    if (result->id != id) {
        fprintf(stderr, "Index is stale; rebuild it before searching.\n");
        invalidate_student_index();
        return -1;
    }

    return 1;
}

void get_index_stats(IndexStats *stats)
{
    if (stats == NULL)
        return;

    stats->entry_count = student_index_count;
    stats->index_bytes = student_index_bytes;
    stats->memory_budget_bytes = student_memory_budget;
}
