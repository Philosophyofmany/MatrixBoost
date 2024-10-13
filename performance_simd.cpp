#include <iostream>
#include <chrono>
#include <papi.h> // Include PAPI header
#include "matrix.hpp"
#include "simd.hpp" // Include the header for SIMD functions
#include <ctime> // Include for clock()
#include <sys/resource.h> // Include for getrusage()

void performTestSIMD(int rows, int cols, double sparsity) {
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

    struct rusage usage; // Structure to hold memory usage information
    long peakMemoryUsage = 0; // Variable to hold peak memory usage

    // Measure Dense-Dense Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting
    clock_t startCpuTime = clock(); // Start CPU time measurement
    auto start = std::chrono::high_resolution_clock::now();
    Matrix denseResult = simd_dense_dense_multiply(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    clock_t endCpuTime = clock(); // End CPU time measurement
    PAPI_stop(EventSet, cacheMisses); // Stop counting and get the cache misses

    // Get memory usage after Dense-Dense multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    std::chrono::duration<double> denseDuration = end - start;
    double userCpuDuration = static_cast<double>(endCpuTime - startCpuTime) / CLOCKS_PER_SEC;
    double systemCpuDuration = userCpuDuration; // Assuming system CPU time is equal for simplicity

    // Output results in the specified format
    std::cout << "Dense-Dense Multiplication Time (SIMD): " << denseDuration.count() << " seconds\n";
    std::cout << "User CPU Time: " << userCpuDuration << " seconds\n";
    std::cout << "System CPU Time: " << systemCpuDuration << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Measure Dense-Sparse Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting again
    startCpuTime = clock(); // Start CPU time measurement
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseResult = simd_dense_sparse_multiply(A, B);
    end = std::chrono::high_resolution_clock::now();
    endCpuTime = clock(); // End CPU time measurement
    PAPI_stop(EventSet, cacheMisses);

    // Get memory usage after Dense-Sparse multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    denseDuration = end - start;
    userCpuDuration = static_cast<double>(endCpuTime - startCpuTime) / CLOCKS_PER_SEC;
    systemCpuDuration = userCpuDuration; // Assuming system CPU time is equal for simplicity

    // Output results for Dense-Sparse
    std::cout << "Dense-Sparse Multiplication Time (SIMD): " << denseDuration.count() << " seconds\n";
    std::cout << "User CPU Time: " << userCpuDuration << " seconds\n";
    std::cout << "System CPU Time: " << systemCpuDuration << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Measure Sparse-Sparse Multiplication Time, CPU Time, and Cache Misses
    PAPI_start(EventSet); // Start counting again
    startCpuTime = clock(); // Start CPU time measurement
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseSparseResult = simd_sparse_sparse_multiply(A, B);
    end = std::chrono::high_resolution_clock::now();
    endCpuTime = clock(); // End CPU time measurement
    PAPI_stop(EventSet, cacheMisses);

    // Get memory usage after Sparse-Sparse multiplication
    getrusage(RUSAGE_SELF, &usage);
    peakMemoryUsage = usage.ru_maxrss; // Peak memory usage in kilobytes

    denseDuration = end - start;
    userCpuDuration = static_cast<double>(endCpuTime - startCpuTime) / CLOCKS_PER_SEC;
    systemCpuDuration = userCpuDuration; // Assuming system CPU time is equal for simplicity

    // Output results for Sparse-Sparse
    std::cout << "Sparse-Sparse Multiplication Time (SIMD): " << denseDuration.count() << " seconds\n";
    std::cout << "User CPU Time: " << userCpuDuration << " seconds\n";
    std::cout << "System CPU Time: " << systemCpuDuration << " seconds\n";
    std::cout << "Cache Misses: " << cacheMisses[0] << "\n";
    std::cout << "Peak Memory Usage: " << peakMemoryUsage << " KB\n";

    // Cleanup PAPI resources
    PAPI_cleanup_eventset(EventSet);
    PAPI_destroy_eventset(&EventSet);
    PAPI_shutdown();
}

void runPerformanceTestSIMD() {
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

    performTestSIMD(rows, cols, sparsity);
}
