#include <iostream>
#include <chrono> // Include for performance measurement
#include "matrix.hpp"
#include <papi.h>
#include "performance_test.cpp"
#include "multithreading.hpp" // Include your multithreading header

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

    // Display matrices
    std::cout << "Matrix A:" << std::endl;
    A.display();
    std::cout << "Matrix B:" << std::endl;
    B.display();

    // Ask the user for the multiplication method
    std::cout << "Choose operation:\n"
              << "1. Dense-Dense Multiplication\n"
              << "2. Dense-Sparse Multiplication\n"
              << "3. Sparse-Sparse Multiplication\n";

    int choice;
    std::cin >> choice;

    // Variable to hold the result
    Matrix result(rowsA, colsB);

    // Ask the user if they want to use multithreading for optimization
    char useMultithreading;
    std::cout << "Do you want to use multithreading for optimization? (y/n): ";
    std::cin >> useMultithreading;

    // Measure performance
    auto start = std::chrono::high_resolution_clock::now(); // Start timing

    // Perform multiplication based on user choice
    if (choice == 1) {
        // Dense-Dense multiplication
        if (useMultithreading == 'y' || useMultithreading == 'Y') {
            result = denseDenseMultiplyThreaded(A, B);
        } else {
            result = A.multiply(B);
        }
        std::cout << "Result of A * B (Dense-Dense):" << std::endl;
    } else if (choice == 2) {
        // Dense-Sparse multiplication
        if (useMultithreading == 'y' || useMultithreading == 'Y') {
            result = denseSparseMultiplyThreaded(A, B);
        } else {
            result = A.multiplySparse(B);
        }
        std::cout << "Result of A * B (Dense-Sparse):" << std::endl;
    } else if (choice == 3) {
        // Sparse-Sparse multiplication
        if (useMultithreading == 'y' || useMultithreading == 'Y') {
            result = sparseSparseMultiplyThreaded(A, B);
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

    // Display the result
    result.display();

    // Ask if the user wants to run the performance test
    char runPerfTest;
    std::cout << "Do you want to run the performance test? (y/n): ";
    std::cin >> runPerfTest;

    if (runPerfTest == 'y' || runPerfTest == 'Y') {
        // Perform the performance test using the same dimensions and sparsity
        performTest(rowsA, colsA, sparsityA);
    }

    return 0;
}
