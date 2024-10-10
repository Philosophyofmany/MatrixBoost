#include "matrix.hpp" // Include your matrix class header
#include <iostream>   // For debug output
#include <algorithm>  // For std::min

// Function to multiply dense matrices using cache optimization (blocking)
Matrix cache_optimized_multiply_dense_dense(const Matrix& A, const Matrix& B) {
    int A_rows = A.getRows();
    int A_cols = A.getCols();
    int B_cols = B.getCols();

    Matrix result(A_rows, B_cols); // Create a result matrix

    const int blockSize = 64; // Example block size

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = 0.0;

                        for (int kk = k; kk < k_end; ++kk) {
                            sum += A.get(ii, kk) * B.get(kk, jj);
                        }

                        result.set(ii, jj, sum); // Directly set the computed sum
                    }
                }
            }
        }
    }

    return result; // Return the result matrix
}

// Function to multiply dense and sparse matrices using cache optimization (blocking)
Matrix cache_optimized_multiply_dense_sparse(const Matrix& A, const Matrix& B) {
    int A_rows = A.getRows();
    int A_cols = A.getCols();
    int B_cols = B.getCols();

    Matrix result(A_rows, B_cols); // Create a result matrix

    const int blockSize = 64; // Example block size

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = 0.0;

                        // Only access non-zero elements of B
                        for (int kk = k; kk < k_end; ++kk) {
                            if (B.isNonZero(kk, jj)) { // Check if the element is non-zero
                                sum += A.get(ii, kk) * B.get(kk, jj);
                            }
                        }

                        result.set(ii, jj, sum); // Directly set the computed sum
                    }
                }
            }
        }
    }

    return result; // Return the result matrix
}

// Function to multiply sparse matrices using cache optimization (blocking)
Matrix cache_optimized_multiply_sparse_sparse(const Matrix& A, const Matrix& B) {
    int A_rows = A.getRows();
    int A_cols = A.getCols();
    int B_cols = B.getCols();

    Matrix result(A_rows, B_cols); // Create a result matrix

    const int blockSize = 64; // Example block size

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = 0.0;

                        // Only access non-zero elements of A and B
                        for (int kk = k; kk < k_end; ++kk) {
                            if (A.isNonZero(ii, kk) && B.isNonZero(kk, jj)) {
                                sum += A.get(ii, kk) * B.get(kk, jj);
                            }
                        }

                        result.set(ii, jj, sum); // Directly set the computed sum
                    }
                }
            }
        }
    }

    return result; // Return the result matrix
}
