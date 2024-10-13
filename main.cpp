#include <iostream>
#include <chrono>
#include "matrix.hpp"
#include <papi.h>
#include "performance_test.cpp"
#include "multithreading.hpp"
#include "simd.cpp" // Include the SIMD header
#include "simd.hpp"
#include "cache_optimization.hpp"
#include "cache_optimization.cpp"
#include "performance_multithreading.cpp"
#include "performance_simd.cpp"
#include "performance_cache.cpp"



int main() {
    int rowsA, colsA, rowsB, colsB;
    double sparsityA, sparsityB;

    // Get matrix A dimensions and sparsity
    std::cout << "Enter number of rows and columns for Matrix A: ";
    std::cin >> rowsA >> colsA;
    std::cout << "Enter sparsity for Matrix A (0.0 to 1.0): ";
    std::cin >> sparsityA;

    // Get matrix B dimensions and sparsity
    std::cout << "Enter number of rows and columns for Matrix B: ";
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

    return 0;
}
