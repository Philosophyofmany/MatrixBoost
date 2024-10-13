#include <iostream>
#include <chrono>
#include <papi.h> // Include PAPI header
#include <sys/resource.h>
#include "matrix.hpp" // Ensure this header is accessible

void runExperimentalResults() {
    // Define matrix sizes (1000, 5000, 7000)
    const int matrixSizes[] = {1000, 1500, 2000};

    // Define sparsities (1.0, 0.01, 0.001) -> dense, 1% sparse, 0.1% sparse
    const double sparsities[] = {1.0, 0.01, 0.001};

    // Initialize PAPI library
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        std::cerr << "PAPI library initialization failed!" << std::endl;
        return;
    }

    // Prepare table headers
    std::cout << "Matrix Size | Sparsity | Dense-Dense Time (s) | Dense-Sparse Time (s) | Sparse-Sparse Time (s) | Cache Misses | Peak Memory (KB)\n";
    std::cout << "--------------------------------------------------------------------------------------------\n";

    // Iterate over matrix sizes and sparsities
    for (int size : matrixSizes) {
        for (double sparsity : sparsities) {
            Matrix A(size, size);
            Matrix B(size, size); // Ensure B dimensions are compatible

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

            struct rusage usage; // For CPU and memory usage statistics

            // Store timings for each type of multiplication
            double denseDenseTime, denseSparseTime, sparseSparseTime;

            // Measure Dense-Dense Multiplication Time
            PAPI_start(EventSet); // Start counting cache misses
            auto start = std::chrono::high_resolution_clock::now();
            Matrix denseResult = A.multiply(B);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            denseDenseTime = duration.count();
            PAPI_stop(EventSet, cacheMisses); // Stop counting cache misses

            // Get peak memory and CPU usage after Dense-Dense multiplication
            getrusage(RUSAGE_SELF, &usage);
            long peakMemory = usage.ru_maxrss;

            // Measure Dense-Sparse Multiplication Time
            PAPI_start(EventSet); // Restart cache miss counting
            start = std::chrono::high_resolution_clock::now();
            Matrix denseSparseResult = A.multiplySparse(B);
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            denseSparseTime = duration.count();
            PAPI_stop(EventSet, cacheMisses); // Stop counting cache misses

            // Get peak memory after Dense-Sparse multiplication
            getrusage(RUSAGE_SELF, &usage);
            peakMemory = std::max(peakMemory, usage.ru_maxrss);

            // Measure Sparse-Sparse Multiplication Time
            PAPI_start(EventSet); // Restart cache miss counting
            start = std::chrono::high_resolution_clock::now();
            Matrix sparseSparseResult = A.multiplySparseSparse(B);
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;
            sparseSparseTime = duration.count();
            PAPI_stop(EventSet, cacheMisses); // Stop counting cache misses

            // Get final peak memory after Sparse-Sparse multiplication
            getrusage(RUSAGE_SELF, &usage);
            peakMemory = std::max(peakMemory, usage.ru_maxrss);

            // Output the collected data as a row in the table
            std::cout << size << "x" << size << " | "
                      << sparsity * 100 << "% | "
                      << denseDenseTime << " | "
                      << denseSparseTime << " | "
                      << sparseSparseTime << " | "
                      << cacheMisses[0] << " | "
                      << peakMemory << " KB\n";

            // Cleanup PAPI resources for this iteration
            PAPI_cleanup_eventset(EventSet);
            PAPI_destroy_eventset(&EventSet);
        }
    }

    // Shutdown PAPI library
    PAPI_shutdown();
}
