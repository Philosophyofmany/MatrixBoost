#include "matrix.hpp"

// Constructor
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    data.resize(r, std::vector<double>(c, 0.0)); // Initialize matrix with zeros
}

// Fill the matrix with random values
void Matrix::fillRandom(double sparsity) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed for random number generation
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Fill with random values based on sparsity
            if (static_cast<double>(std::rand()) / RAND_MAX >= sparsity) {
                data[i][j] = static_cast<double>(std::rand()) / RAND_MAX * 10; // Random value between 0 and 10
            } else {
                data[i][j] = 0.0; // Sparse element
            }
        }
    }
}

// Display the matrix
void Matrix::display() const {
    for (const auto& row : data) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

// Dense-Dense multiplication
Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}
