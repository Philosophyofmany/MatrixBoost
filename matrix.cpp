#include "matrix.hpp"


// Get number of rows
int Matrix::getRows() const {
    return rows;
}

// Get number of columns
int Matrix::getCols() const {
    return cols;
}

// Access individual elements
double Matrix::get(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[row][col];
}


// Set individual elements
void Matrix::set(int row, int col, double value) {
    data[row][col] = value;
}

// Constructor
Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols)); // Ensure this is correct
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

// Dense-Sparse multiplication
Matrix Matrix::multiplySparse(const Matrix& sparseMatrix) const {
    if (cols != sparseMatrix.rows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(rows, sparseMatrix.cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < sparseMatrix.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                if (sparseMatrix.data[k][j] != 0) { // Only multiply if the element is non-zero
                    result.data[i][j] += data[i][k] * sparseMatrix.data[k][j];
                }
            }
        }
    }
    return result;
}

// Sparse-Sparse multiplication
Matrix Matrix::multiplySparseSparse(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(rows, other.cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols; ++k) {
                // Only multiply non-zero elements
                if (data[i][k] != 0 && other.data[k][j] != 0) {
                    sum += data[i][k] * other.data[k][j];
                }
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}
