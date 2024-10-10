#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <cstdlib> // For std::rand and std::srand
#include <ctime>   // For std::time

class Matrix {
public:
    // Constructor
    Matrix(int r, int c);

    // Function to fill the matrix with random values (for testing)
    void fillRandom(double sparsity = 0.0); // Sparsity between 0 and 1

    // Function to display the matrix (for testing)
    void display() const;

    // Dense-Dense multiplication
    Matrix multiply(const Matrix& other) const;

    // Dense-Sparse multiplication
    Matrix multiplySparse(const Matrix& other) const;

    // Sparse-Sparse multiplication
    Matrix multiplySparseSparse(const Matrix& other) const;

    // Add other multiplication methods as needed
    // Get number of rows
    int getRows() const;

    // Get number of columns
    int getCols() const;

    // Access individual elements (optional, but useful)
    double get(int row, int col) const;

    // Set individual elements (optional, but useful)
    void set(int row, int col, double value);

private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data; // Matrix data
};

#endif // MATRIX_HPP
