#include <iostream>
#include <chrono>
#include <papi.h> // Include PAPI header
#include "matrix.hpp" // Ensure this header is accessible
#include "multithreading.hpp"

void performTestMultithreading(int rows, int cols, double sparsity) {
    // Initialize PAPI library
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        std::cerr << "PAPI library initialization failed!" << std::endl;
        return;
    }

    Matrix A(rows, cols);
    Matrix B(cols, rows); // Ensure B dimensions are compatible

    // Fill matrices with random values based on sparsity
    A.fillRandom(sparsity);
    B.fillRandom(sparsity);

    long long cacheMisses[1]; // Array to hold the number of cache misses
    int EventSet = PAPI_NULL; // Initialize PAPI Event Set

    // Create the event set
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        std::cerr << "PAPI create event set failed!" << std::endl;
        return;
    }

    // Add the L1 data cache miss event to the event set
    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK) {
        std::cerr << "PAPI add event failed!" << std::endl;
        return;
    }

    // Measure Dense-Dense Multiplication Time and Cache Misses
    PAPI_start(EventSet); // Start counting
    auto start = std::chrono::high_resolution_clock::now();
    Matrix denseResult = denseDenseMultiplyThreaded(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> denseDuration = end - start;
    PAPI_stop(EventSet, cacheMisses); // Stop counting and get the cache misses

    std::cout << "Dense-Dense Multiplication Time (Threaded): " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";

    // Measure Dense-Sparse Multiplication Time and Cache Misses
    PAPI_start(EventSet); // Start counting again
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseResult = denseSparseMultiplyThreaded(A, B);
    end = std::chrono::high_resolution_clock::now();
    denseDuration = end - start;
    PAPI_stop(EventSet, cacheMisses);

    std::cout << "Dense-Sparse Multiplication Time (Threaded): " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";

    // Measure Sparse-Sparse Multiplication Time and Cache Misses
    PAPI_start(EventSet); // Start counting again
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseSparseResult = sparseSparseMultiplyThreaded(A, B);
    end = std::chrono::high_resolution_clock::now();
    denseDuration = end - start;
    PAPI_stop(EventSet, cacheMisses);

    std::cout << "Sparse-Sparse Multiplication Time (Threaded): " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";

    // Cleanup PAPI resources
    PAPI_cleanup_eventset(EventSet);
    PAPI_destroy_eventset(&EventSet);
    PAPI_shutdown();
}

void runPerformanceTestMultithreading() {
    int rows, cols;
    double sparsity;

    std::cout << "Enter number of rows for Matrix A: ";
    std::cin >> rows;
    std::cout << "Enter number of columns for Matrix A (and rows for Matrix B): ";
    std::cin >> cols;

    // Input validation for sparsity
    do {
        std::cout << "Enter sparsity level (0.0 for dense, 1.0 for sparse): ";
        std::cin >> sparsity;
    } while (sparsity < 0.0 || sparsity > 1.0);

    performTestMultithreading(rows, cols, sparsity);
}
