#include <iostream>
#include <chrono>
#include "matrix.hpp"
#include <papi.h>
#include "performance_test.cpp"
#include "multithreading.hpp"
#include "simd.cpp"
#include "simd.hpp"
#include "cache_optimization.hpp"
#include "cache_optimization.cpp"
#include "performance_multithreading.cpp"
#include "performance_simd.cpp"
#include "performance_cache.cpp"
#include "experimental_results.cpp"
#include "experimental_multithreading.cpp"

// Running tests
void optimizationTest() {
    int sizes[3][2] = {
        {500, 500},
        {700, 700},
        {800, 800}
    }; // Matrix dimensions (rows, cols)

    double sparsitiesA[4] = {0.01, 0.1, 0.5, 1.0}; // Sparsity for Matrix A
    double sparsitiesB[4] = {0.01, 0.1, 0.5, 1.0}; // Sparsity for Matrix B

    for (auto size : sizes) {
        int rows = size[0];
        int cols = size[1];

        for (double sparsityA : sparsitiesA) {
            for (double sparsityB : sparsitiesB) {
                Matrix A(rows, cols);
                Matrix B(rows, cols);

                A.fillRandom(sparsityA);
                B.fillRandom(sparsityB);

                std::cout << "Testing with size (" << rows << "x" << cols << ") - "
                          << "Sparsity A: " << sparsityA << ", "
                          << "Sparsity B: " << sparsityB << std::endl;

                // Measure performance for each optimization
                char optimizations[3] = {'m', 's', 'c'}; // multithreading, SIMD, cache optimization

                for (char opt : optimizations) {
                    auto start = std::chrono::high_resolution_clock::now();

                    // Perform multiplication based on the chosen optimization
                    if (opt == 'm') {
                        Matrix result = denseDenseMultiplyThreaded(A, B);
                    } else if (opt == 's') {
                        Matrix result = simd_dense_dense_multiply(A, B);
                    } else if (opt == 'c') {
                        Matrix result = cache_optimized_multiply_dense_dense(A, B);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start; // Calculate duration
                    std::cout << "Time taken for optimization " << opt << ": "
                              << duration.count() << " seconds" << std::endl;
                }
            }
        }
    }
}


int main() {
    int rowsA, colsA, rowsB, colsB;
    double sparsityA, sparsityB;

    // Get matrix A dimensions and sparsity
    std::cout << "Enter number of rows and columns for Matrix A (ex: 10 10): ";
    std::cin >> rowsA >> colsA;
    std::cout << "Enter sparsity for Matrix A (0.0 to 1.0): ";
    std::cin >> sparsityA;

    // Get matrix B dimensions and sparsity
    std::cout << "Enter number of rows and columns for Matrix B (ex: 10 10): ";
    std::cin >> rowsB >> colsB;
    std::cout << "Enter sparsity for Matrix B (0.0 to 1.0): ";
    std::cin >> sparsityB;

    // Ensure compatible dimensions for multiplication
    if (colsA != rowsB) {
        std::cerr << "Error: Incompatible dimensions for multiplication!" << std::endl;
        return 1;
    }

    // Create matrices
    Matrix A(rowsA, colsA);
    Matrix B(rowsB, colsB);

    // Fill matrices with random values
    A.fillRandom(sparsityA);
    B.fillRandom(sparsityB);



    // // Print the matrices
    // std::cout << "Matrix A:" << std::endl;
    // A.display(); // Print Matrix A
    // std::cout << "Matrix B:" << std::endl;
    // B.display(); // Print Matrix B

    // Ask the user for the multiplication method
    std::cout << "Choose operation:\n"
              << "1. Dense-Dense Multiplication\n"
              << "2. Dense-Sparse Multiplication\n"
              << "3. Sparse-Sparse Multiplication\n";

    int choice;
    std::cin >> choice;

    // Variable to hold the result
    Matrix result(rowsA, colsB);

    // Ask the user if they want to use multithreading, SIMD, or cache optimization for optimization
    char useOptimization;
    std::cout << "Do you want to use optimization? (m for multithreading, s for SIMD, c for cache optimization, n for none): ";
    std::cin >> useOptimization;

    // Measure performance
    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    // Perform multiplication based on user choice
    if (choice == 1) {
        // Dense-Dense multiplication
        if (useOptimization == 'm' || useOptimization == 'M') {
            result = denseDenseMultiplyThreaded(A, B);
        } else if (useOptimization == 's' || useOptimization == 'S') {
            result = simd_dense_dense_multiply(A, B);
        } else if (useOptimization == 'c' || useOptimization == 'C') {
            result = cache_optimized_multiply_dense_dense(A, B);
        } else {
            result = A.multiply(B);
        }
        std::cout << "Result of A * B (Dense-Dense):" << std::endl;
    } else if (choice == 2) {
        // Dense-Sparse multiplication
        if (useOptimization == 'm' || useOptimization == 'M') {
            result = denseSparseMultiplyThreaded(A, B);
        } else if (useOptimization == 's' || useOptimization == 'S') {
            result = simd_dense_sparse_multiply(A, B);
        } else if (useOptimization == 'c' || useOptimization == 'C') {
            result = cache_optimized_multiply_dense_sparse(A, B);
        } else {
            result = A.multiplySparse(B);
        }
        std::cout << "Result of A * B (Dense-Sparse):" << std::endl;
    } else if (choice == 3) {
        // Sparse-Sparse multiplication
        if (useOptimization == 'm' || useOptimization == 'M') {
            result = sparseSparseMultiplyThreaded(A, B);
        } else if (useOptimization == 's' || useOptimization == 'S') {
            result = simd_sparse_sparse_multiply(A, B);
        } else if (useOptimization == 'c' || useOptimization == 'C') {
            result = cache_optimized_multiply_sparse_sparse(A, B);
        } else {
            result = A.multiplySparseSparse(B);
        }
        std::cout << "Result of A * B (Sparse-Sparse):" << std::endl;
    } else {
        std::cerr << "Invalid choice!" << std::endl;
        return 1;
    }

    // Stop timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start; // Calculate duration
    std::cout << "Time taken for multiplication: " << duration.count() << " seconds" << std::endl;

    // Ask if the user wants to run the performance test
    char runPerfTest;
    std::cout << "Do you want to run the performance test? (y/n): ";
    std::cin >> runPerfTest;

    if (runPerfTest == 'y' || runPerfTest == 'Y') {
        // Run the appropriate performance test based on the optimization used
        if (useOptimization == 'm' || useOptimization == 'M') {
            std::cout << "Running multithreading performance test...\n";
            performTestMultithreading(rowsA, colsA, sparsityA);
        } else if (useOptimization == 's' || useOptimization == 'S') {
            std::cout << "Running SIMD performance test...\n";
            performTestSIMD(rowsA, colsA, sparsityA);
        } else if (useOptimization == 'c' || useOptimization == 'C') {
            std::cout << "Running cache optimization performance test...\n";
            performCacheOptimizedTest(rowsA, colsA, sparsityA);
        } else {
            std::cout << "Running default performance test...\n";
            performTest(rowsA, colsA, sparsityA);
        }
    }

    // Ask if the user wants to view optimization test results
    char viewOptTestResults;
    std::cout << "Do you want to view the optimization test results? (y/n): ";
    std::cin >> viewOptTestResults;

    if (viewOptTestResults == 'y' || viewOptTestResults == 'Y') {
        // Add code to display the optimization test results
        // This could be a call to a function that shows the results or prints the data
        optimizationTest(); // Assume this function is defined elsewhere
    }


    // Ask if the user wants to run experimental results
    char runExperimental;
    std::cout << "Do you want to run experimental results? (y/n): ";
    std::cin >> runExperimental;

    // Run experimental results if the user chooses to
    if (runExperimental == 'y' || runExperimental == 'Y') {
        std::cout << "Running experimental results..." << std::endl;
        runExperimentalResults();  // Run the experimental results
    }

    char runExperimental_multithreading;
    std::cout << "Do you want to run experimental mode (multithreading)? (y/n): ";
    std::cin >> runExperimental_multithreading;

    // Run experimental results if the user chooses to
    if (runExperimental_multithreading == 'y' || runExperimental_multithreading == 'Y') {
        // Get matrix dimensions for experimental multiplication
        int rowsA_exp, colsA_exp, rowsB_exp, colsB_exp;
        std::cout << "Enter number of rows and columns for Matrix A (experimental): ";
        std::cin >> rowsA_exp >> colsA_exp;
        std::cout << "Enter number of rows and columns for Matrix B (experimental): ";
        std::cin >> rowsB_exp >> colsB_exp;

        // Ensure compatible dimensions for multiplication
        if (colsA_exp != rowsB_exp) {
            std::cerr << "Error: Incompatible dimensions for multiplication!" << std::endl;
            return 1;
        }

        // Create matrices for experimental multiplication
        Matrix A_exp(rowsA_exp, colsA_exp);
        Matrix B_exp(rowsB_exp, colsB_exp);

        // Fill matrices with random values (assuming sparsity is handled)
        double sparsityA_exp, sparsityB_exp;
        std::cout << "Enter sparsity for Matrix A (0.0 to 1.0): ";
        std::cin >> sparsityA_exp;
        A_exp.fillRandom(sparsityA_exp);

        std::cout << "Enter sparsity for Matrix B (0.0 to 1.0): ";
        std::cin >> sparsityB_exp;
        B_exp.fillRandom(sparsityB_exp);

        // Choose the multiplication type based on user preference
        // Timing Dense-Dense multiplication
        auto start = std::chrono::high_resolution_clock::now();
        Matrix resultDenseDense = experimentalDenseDenseMultiply(A_exp, B_exp);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationDenseDense = end - start;
        std::cout << "Completed Dense-Dense multiplication in " << durationDenseDense.count() << " seconds." << std::endl;

        // Timing Dense-Sparse multiplication
        start = std::chrono::high_resolution_clock::now();
        Matrix resultDenseSparse = experimentalDenseSparseMultiply(A_exp, B_exp);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationDenseSparse = end - start;
        std::cout << "Completed Dense-Sparse multiplication in " << durationDenseSparse.count() << " seconds." << std::endl;

        // Timing Sparse-Sparse multiplication
        start = std::chrono::high_resolution_clock::now();
        Matrix resultSparseSparse = experimentalSparseSparseMultiply(A_exp, B_exp);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationSparseSparse = end - start;
        std::cout << "Completed Sparse-Sparse multiplication in " << durationSparseSparse.count() << " seconds." << std::endl;

        // Output the result or handle it as needed
        std::cout << "Multiplication completed." << std::endl;
        // You can add code here to display or process the result matrix
    }


    return 0;
}
