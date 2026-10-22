
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>

#include "student.h"
#include "sequential_search.h"
#include "indexed_search.h"

#define DEFAULT_MEMORY_BUDGET 1024

static size_t memory_budget = DEFAULT_MEMORY_BUDGET;

/* Remove leftover characters from the current input line. */
static void clear_input_line(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void configure_memory(void)
{
    unsigned long long kb;

    printf("Enter memory budget in KB: ");

    if (scanf("%llu", &kb) != 1) {
        printf("Invalid input.\n");
        clear_input_line();
        return;
    }

    clear_input_line();

    if (kb > (unsigned long long)((size_t)-1) / 1024) {
        printf("Memory budget is too large.\n");
        return;
    }

    memory_budget = (size_t)kb * 1024;
    invalidate_student_index();

    printf("Memory budget set to %zu bytes.\n", memory_budget);
}

static int read_student_id(int *id)
{
    printf("Enter student ID: ");

    if (scanf("%d", id) != 1) {
        printf("Invalid ID.\n");
        clear_input_line();
        return 0;
    }

    clear_input_line();

    if (*id <= 0) {
        printf("ID must be positive.\n");
        return 0;
    }

    return 1;
}

static void sequential_search_menu(void)
{
    int id;
    int examined = 0;
    int status;
    double elapsed = 0.0;
    Student result;

    if (!read_student_id(&id))
        return;

    status = sequential_search(id, &result,
                               &examined, &elapsed);

    if (status == 1)
        printf("Found: %d, %s, %.2f\n",
               result.id, result.name, result.marks);
    else if (status == 0)
        printf("Student not found.\n");
    else
        printf("Sequential search failed.\n");

    printf("Records examined: %d\n", examined);
    printf("Elapsed time: %.9f seconds\n", elapsed);
}

static void indexed_search_menu(void)
{
    int id;
    int build_status;
    int status;
    size_t comparisons = 0;
    Student result;
    IndexStats stats;

    build_status = build_student_index(memory_budget);

    if (build_status != 1) {
        if (build_status == 0)
            printf("Index unavailable: insufficient memory.\n");
        else
            printf("Could not build the index.\n");
        return;
    }

    if (!read_student_id(&id))
        return;

    status = indexed_search(id, &result, &comparisons);

    if (status == 1)
        printf("Found: %d, %s, %.2f\n",
               result.id, result.name, result.marks);
    else if (status == 0)
        printf("Student not found.\n");
    else
        printf("Indexed search failed. Rebuild the index if needed.\n");

    get_index_stats(&stats);

    printf("Index comparisons: %zu\n", comparisons);
    printf("Index entries: %zu\n", stats.entry_count);
    printf("Index memory: %zu bytes\n", stats.index_bytes);
}

static void compare_searches(void)
{
    int id;
    int sequential_examined = 0;
    int sequential_status;
    int indexed_status;
    double sequential_time = 0.0;
    double indexed_time;
    clock_t start, end;
    size_t index_comparisons = 0;
    int build_status;

    Student sequential_result;
    Student indexed_result;
    IndexStats stats;

    if (!read_student_id(&id))
        return;

    /* Run sequential search first. */
    sequential_status = sequential_search(
        id, &sequential_result,
        &sequential_examined, &sequential_time
    );

    /* Build a fresh index before the indexed search. */
    build_status = build_student_index(memory_budget);

    if (build_status != 1) {
        printf("Cannot compare: index could not be built.\n");
        return;
    }

    start = clock();

    indexed_status = indexed_search(
        id, &indexed_result, &index_comparisons
    );

    end = clock();

    indexed_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n========== SEARCH COMPARISON ==========\n");
    printf("%-28s %-18s %-18s\n",
           "Metric", "Sequential", "Indexed");
    printf("------------------------------------------------------------\n");

    printf("%-28s %-18s %-18s\n",
           "Search result",
           sequential_status == 1 ? "Found" :
           sequential_status == 0 ? "Not found" : "Error",
           indexed_status == 1 ? "Found" :
           indexed_status == 0 ? "Not found" : "Error");

    printf("%-28s %-18d %-18zu\n",
           "Operations counted",
           sequential_examined, index_comparisons);

    printf("%-28s %-18.9f %-18.9f\n",
           "Search time (seconds)",
           sequential_time, indexed_time);

    get_index_stats(&stats);

    printf("Index memory required: %zu bytes\n", stats.index_bytes);
    printf("Configured memory budget: %zu bytes\n", memory_budget);

    printf("\nNote: sequential operations count records examined; "
           "indexed operations count binary-search comparisons.\n");
    printf("Indexed timing includes the indexed_search function call, "
           "but excludes index construction.\n");
    printf("Sequential timing follows the timing interval defined "
           "inside the sequential-search module.\n");
}

int main(void)
{
    int choice;

    do {
        printf("\n===== OS FILE ACCESS ANALYSIS =====\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Sequential Search\n");
        printf("4. Build Index and Indexed Search\n");
        printf("5. Configure Memory Budget\n");
        printf("6. Compare Both Methods\n");
        printf("7. Display Performance Results\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number from 1 to 8.\n");
            clear_input_line();
            continue;
        }

        clear_input_line();

        switch (choice) {
            case 1:
                add_student_interactive();
                /* Adding a record may make the index stale. */
                invalidate_student_index();
                break;

            case 2:
                display_students();
                break;

            case 3:
                sequential_search_menu();
                break;

            case 4:
                indexed_search_menu();
                break;

            case 5:
                configure_memory();
                break;

            case 6:
                compare_searches();
                break;


            case 7: {
                IndexStats stats;
                int status;

                status = build_student_index(memory_budget);
                get_index_stats(&stats);

                printf("\n===== PERFORMANCE RESULTS =====\n");
                printf("Configured memory budget: %zu bytes\n", memory_budget);
                printf("Records indexed: %zu\n", stats.entry_count);
                printf("Index memory required: %zu bytes\n", stats.index_bytes);

                if (status == 1) {
                    printf("Index status: Ready\n");
                } else if (status == 0) {
                    printf("Index status: Not built (insufficient memory)\n");
                } else {
                    printf("Index status: Build failed (file or other error)\n");
                }

                printf("Note: Search performance measurements are displayed in Option 6.\n");
                break;
            }


            case 8:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Select 1-8.\n");
        }

    } while (choice != 8);

    return 0;
}
