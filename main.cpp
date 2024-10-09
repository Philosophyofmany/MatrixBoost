#include <iostream>
#include "matrix.hpp"

int main() {
    int rows, cols;

    // Prompt user for matrix dimensions
    std::cout << "Enter the number of rows for matrices A and B: ";
    std::cin >> rows;
    std::cout << "Enter the number of columns for matrices A and B: ";
    std::cin >> cols;

    // Create Matrix objects A and B with specified dimensions
    Matrix A(rows, cols);
    Matrix B(rows, cols);

    // Fill matrices with random values (you can also implement sparsity if needed)
    A.fillRandom(0.1); // 10% non-zero elements
    B.fillRandom(0.1); // 10% non-zero elements

    // Display matrices (optional)
    std::cout << "Matrix A:" << std::endl;
    A.display();
    std::cout << "Matrix B:" << std::endl;
    B.display();

    // Perform operations based on user choice
    int operation;
    std::cout << "Choose operation:\n1. Dense-Dense Multiplication\n";
    std::cin >> operation;

    if (operation == 1) {
        Matrix C = A.multiply(B);
        std::cout << "Result of A * B:" << std::endl;
        C.display();
    }

    return 0;
}
