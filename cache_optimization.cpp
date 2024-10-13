#include "matrix.hpp" // Include your matrix class header
#include <iostream>   // For debug output
#include <algorithm>  // For std::min

// Function to multiply dense matrices using cache optimization (blocking)
Matrix cache_optimized_multiply_dense_dense(const Matrix& A, const Matrix& B) {
    int A_rows = A.getRows();
    int A_cols = A.getCols();
    int B_cols = B.getCols();

    Matrix result(A_rows, B_cols); // Create a result matrix initialized to zero

    const int blockSize = 64; // Example block size optimized for cache

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                // Multiply the blocks
                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = result.get(ii, jj); // Get current value in result

                        for (int kk = k; kk < k_end; ++kk) {
                            sum += A.get(ii, kk) * B.get(kk, jj); // Multiply and accumulate
                        }

                        result.set(ii, jj, sum); // Update result
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

    Matrix result(A_rows, B_cols); // Create a result matrix initialized to zero

    const int blockSize = 64; // Example block size optimized for cache

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                // Multiply the blocks
                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = result.get(ii, jj); // Get current value in result

                        // Only access non-zero elements of sparse matrix B
                        for (int kk = k; kk < k_end; ++kk) {
                            if (B.isNonZero(kk, jj)) { // Only check non-zero elements of sparse matrix
                                sum += A.get(ii, kk) * B.get(kk, jj); // Multiply and accumulate
                            }
                        }

                        result.set(ii, jj, sum); // Update result
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

    Matrix result(A_rows, B_cols); // Create a result matrix initialized to zero

    const int blockSize = 64; // Example block size optimized for cache

    for (int i = 0; i < A_rows; i += blockSize) {
        for (int j = 0; j < B_cols; j += blockSize) {
            for (int k = 0; k < A_cols; k += blockSize) {
                int i_end = std::min(i + blockSize, A_rows);
                int j_end = std::min(j + blockSize, B_cols);
                int k_end = std::min(k + blockSize, A_cols);

                // Multiply the blocks
                for (int ii = i; ii < i_end; ++ii) {
                    for (int jj = j; jj < j_end; ++jj) {
                        double sum = result.get(ii, jj); // Get current value in result

                        // Only access non-zero elements of sparse matrices A and B
                        for (int kk = k; kk < k_end; ++kk) {
                            if (A.isNonZero(ii, kk) && B.isNonZero(kk, jj)) {
                                sum += A.get(ii, kk) * B.get(kk, jj); // Multiply and accumulate
                            }
                        }

                        result.set(ii, jj, sum); // Update result
                    }
                }
            }
        }
    }

    return result; // Return the result matrix
}
