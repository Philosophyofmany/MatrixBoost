#include <iostream>
#include <chrono>
#include "matrix.hpp" // Ensure this header is accessible

void performTest(int rows, int cols, double sparsity) {
    Matrix A(rows, cols);
    Matrix B(cols, rows); // B should be compatible for multiplication with A

    // Fill matrices with random values based on sparsity
    A.fillRandom(sparsity);
    B.fillRandom(sparsity);

    // Measure Dense-Dense Multiplication Time
    auto start = std::chrono::high_resolution_clock::now();
    Matrix denseResult = A.multiply(B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> denseDuration = end - start;
    std::cout << "Dense-Dense Multiplication Time: " << denseDuration.count() << " seconds\n";

    // Measure Dense-Sparse Multiplication Time
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseResult = A.multiplySparse(B);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sparseDuration = end - start;
    std::cout << "Dense-Sparse Multiplication Time: " << sparseDuration.count() << " seconds\n";

    // Measure Sparse-Sparse Multiplication Time
    start = std::chrono::high_resolution_clock::now();
    Matrix sparseSparseResult = A.multiplySparseSparse(B);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sparseSparseDuration = end - start;
    std::cout << "Sparse-Sparse Multiplication Time: " << sparseSparseDuration.count() << " seconds\n";
}

void runPerformanceTest() {
    int rows, cols;
    double sparsity;

    std::cout << "Enter number of rows for Matrix A: ";
    std::cin >> rows;
    std::cout << "Enter number of columns for Matrix A (and rows for Matrix B): ";
    std::cin >> cols;
    std::cout << "Enter sparsity level (0.0 for dense, 1.0 for sparse): ";
    std::cin >> sparsity;

    performTest(rows, cols, sparsity);
}
