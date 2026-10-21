# Comparative Analysis of Sequential and Indexed File Access Under Limited Memory Using C

## Project Overview

This project compares sequential and indexed file access methods through a student record management system developed in C. It explores how different file access techniques affect search performance and memory usage under simulated memory constraints.

## Objectives

- Implement student record storage and retrieval using file handling in C.
- Perform sequential searching by examining records one by one.
- Implement indexed searching using student IDs and file positions.
- Simulate a limited memory budget and evaluate whether the index fits within it.
- Compare both methods based on records examined, search time, and index memory requirements.

## Key Features

- Add and display student records containing ID, name, and marks.
- Search for student records using sequential and indexed access.
- Build and search a sorted index using binary search.
- Simulate memory constraints for index storage.
- Compare the efficiency and memory overhead of both approaches.
- Handle duplicate IDs, invalid input, missing records, and insufficient memory.

## Technologies Used

- **Programming Language:** C
- **Concepts:** File Handling, Searching Algorithms, Indexing, Memory Management
- **Development Environment:** GCC or Clang with Visual Studio Code

## Project Methodology

**Sequential Access:** Records are checked one by one until the requested student is found or the end of the file is reached.

**Indexed Access:** A separate index stores student IDs and their corresponding file positions. The index is sorted by ID, allowing binary search to locate a record's position more efficiently.

**Memory Simulation:** The index's estimated memory requirement is calculated and compared against a user-defined memory budget. This simulates a resource constraint; it does not control the operating system's actual memory allocation.

**Performance Comparison:** Both methods are evaluated using the same student dataset, considering search effort, elapsed time, and index memory overhead.

## Team Contributions

The project is divided into four modules:

1. **Member 1 — File Handling and Records:** Student record structure, input validation, file storage, and record display.
2. **Member 2 — Sequential Search:** Record-by-record searching and performance measurement.
3. **Member 3 — Indexed Search and Memory Simulation:** Index construction, binary search, and memory-budget checks.
4. **Member 4 — Integration and Testing:** Main menu, module integration, comparison results, and overall testing.

## Expected Outcome

The project demonstrates the trade-offs between sequential and indexed file access. Sequential searching has minimal indexing overhead, while indexed searching can reduce the number of search operations at the cost of additional memory and index-building work. Actual performance depends on the dataset, implementation, and test environment.

## Academic Context

This project applies concepts from Operating Systems, particularly file access methods and memory management, through a practical C programming implementation.
