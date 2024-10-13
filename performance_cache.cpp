#include <iostream>
#include <chrono>
#include <papi.h> // Include PAPI header
#include "cache_optimization.hpp" // Include the cache-optimized functions
#include <sys/resource.h> // Include for getrusage()

void performCacheOptimizedTest(int rows, int cols, double sparsity) {
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

    struct rusage usage; // Structure to hold memory usage and CPU time information
    long peakMemoryUsage = 0; // Variable to hold peak memory usage

    // Measure Cache-Optimized Dense-Dense Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting
    auto start = std::chrono::high_resolution_clock::now();
    Matrix denseDenseResult = cache_optimized_multiply_dense_dense(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    PAPI_stop(EventSet, cacheMisses); // Stop counting and get the cache misses

    // Get memory usage and CPU times after Dense-Dense multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    std::chrono::duration<double> denseDuration = end - start;

    std::cout << "Cache-Optimized Dense-Dense Multiplication Time: " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "User CPU Time: " << usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6 << " seconds\n";
    std::cout << "System CPU Time: " << usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6 << " seconds\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Measure Cache-Optimized Dense-Sparse Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting again
    start = std::chrono::high_resolution_clock::now();
    Matrix denseSparseResult = cache_optimized_multiply_dense_sparse(A, B);
    end = std::chrono::high_resolution_clock::now();
    PAPI_stop(EventSet, cacheMisses);

    // Get memory usage and CPU times after Dense-Sparse multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    denseDuration = end - start;

    std::cout << "Cache-Optimized Dense-Sparse Multiplication Time: " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "User CPU Time: " << usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6 << " seconds\n";
    std::cout << "System CPU Time: " << usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6 << " seconds\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Measure Cache-Optimized Sparse-Sparse Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting again
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseSparseResult = cache_optimized_multiply_sparse_sparse(A, B);
    end = std::chrono::high_resolution_clock::now();
    PAPI_stop(EventSet, cacheMisses);

    // Get memory usage and CPU times after Sparse-Sparse multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    denseDuration = end - start;

    std::cout << "Cache-Optimized Sparse-Sparse Multiplication Time: " << denseDuration.count() << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "User CPU Time: " << usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6 << " seconds\n";
    std::cout << "System CPU Time: " << usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6 << " seconds\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Cleanup PAPI resources
    PAPI_cleanup_eventset(EventSet);
    PAPI_destroy_eventset(&EventSet);
    PAPI_shutdown();
}

void runCacheOptimizedPerformanceTest() {
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

    performCacheOptimizedTest(rows, cols, sparsity);
}
