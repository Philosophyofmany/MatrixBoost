#include "matrix.hpp"
#include <iostream>
#include "performance_test.cpp"
#include <chrono> // For timing



int main() {
    int rowsA, colsA, rowsB, colsB;
    double sparsityA, sparsityB;

    std::cout << "Enter number of rows and columns for Matrix A: ";
    std::cin >> rowsA >> colsA;

    std::cout << "Enter sparsity for Matrix A (0.0 to 1.0): ";
    std::cin >> sparsityA;

    std::cout << "Enter number of rows and columns for Matrix B: ";
    std::cin >> rowsB >> colsB;

    std::cout << "Enter sparsity for Matrix B (0.0 to 1.0): ";
    std::cin >> sparsityB;

    Matrix A(rowsA, colsA);
    Matrix B(rowsB, colsB);

    // Fill matrices with random values based on the specified sparsity
    A.fillRandom(sparsityA);
    B.fillRandom(sparsityB);

    std::cout << "Matrix A:\n";
    A.display();
    std::cout << "Matrix B:\n";
    B.display();

    // Choose operation
    int choice;
    std::cout << "Choose operation:\n";
    std::cout << "1. Dense-Dense Multiplication\n";
    std::cout << "2. Dense-Sparse Multiplication\n";
    std::cout << "3. Sparse-Sparse Multiplication\n";
    std::cin >> choice;

    if (choice == 1) {
        Matrix result = A.multiply(B);
        std::cout << "Result of A * B (Dense-Dense):\n";
        result.display();
    } else if (choice == 2) {
        Matrix result = A.multiplySparse(B);
        std::cout << "Result of A * B (Dense-Sparse):\n";
        result.display();
    } else if (choice == 3) {
        Matrix result = A.multiplySparseSparse(B);
        std::cout << "Result of A * B (Sparse-Sparse):\n";
        result.display();
    } else {
        std::cout << "Invalid choice!\n";
    }

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
