#include "simd.hpp"
#include <immintrin.h> // For AVX

// Function to multiply a single row of A with B using AVX for dense-dense multiplication
void simd_multiplyRowDenseDense(const Matrix& A, const Matrix& B, Matrix& result, int row) {
    for (int col = 0; col < B.getCols(); ++col) {
        __m256d sum = _mm256_setzero_pd(); // Initialize sum to zero

        for (int k = 0; k < A.getCols(); k += 4) { // Increment k by 4 for AVX
            // Create temporary arrays to hold data
            double a_data[4];
            double b_data[4];

            // Load 4 elements into temporary arrays from A and B
            for (int i = 0; i < 4; ++i) {
                if (k + i < A.getCols()) {
                    a_data[i] = A.get(row, k + i); // Get values from A
                } else {
                    a_data[i] = 0.0; // Padding with zeros if out of bounds
                }
                if (k + i < B.getRows()) {
                    b_data[i] = B.get(k + i, col); // Get values from B
                } else {
                    b_data[i] = 0.0; // Padding with zeros if out of bounds
                }
            }

            // Load data into AVX registers
            __m256d a = _mm256_loadu_pd(a_data); // Load 4 elements from temporary array
            __m256d b = _mm256_loadu_pd(b_data); // Load 4 elements from temporary array
            sum = _mm256_add_pd(sum, _mm256_mul_pd(a, b)); // Multiply and accumulate
        }

        double result_arr[4];
        _mm256_storeu_pd(result_arr, sum); // Store the result
        result.set(row, col, result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3]); // Sum all results
    }
}

// Dense-Dense multiplication using AVX
Matrix simd_dense_dense_multiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        simd_multiplyRowDenseDense(A, B, result, i);
    }

    return result;
}

// Function to multiply a single row of A with B using AVX for dense-sparse multiplication
void simd_multiplyRowDenseSparse(const Matrix& A, const Matrix& B, Matrix& result, int row) {
    for (int col = 0; col < B.getCols(); ++col) {
        __m256d sum = _mm256_setzero_pd(); // Initialize sum to zero

        for (int k = 0; k < A.getCols(); k += 4) { // Increment k by 4 for AVX
            // Create temporary arrays to hold data
            double a_data[4];
            double b_data[4];

            // Load 4 elements into temporary arrays from A and B
            for (int i = 0; i < 4; ++i) {
                if (k + i < A.getCols()) {
                    a_data[i] = A.get(row, k + i); // Get values from A
                } else {
                    a_data[i] = 0.0; // Padding with zeros if out of bounds
                }
                if (k + i < B.getRows()) {
                    b_data[i] = B.get(k + i, col); // Get values from B
                } else {
                    b_data[i] = 0.0; // Padding with zeros if out of bounds
                }
            }

            // Load data into AVX registers
            __m256d a = _mm256_loadu_pd(a_data); // Load 4 elements from temporary array
            __m256d b = _mm256_loadu_pd(b_data); // Load 4 elements from temporary array
            sum = _mm256_add_pd(sum, _mm256_mul_pd(a, b)); // Multiply and accumulate
        }

        double result_arr[4];
        _mm256_storeu_pd(result_arr, sum); // Store the result
        result.set(row, col, result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3]); // Sum all results
    }
}

// Dense-Sparse multiplication using AVX
Matrix simd_dense_sparse_multiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        simd_multiplyRowDenseSparse(A, B, result, i);
    }

    return result;
}

// Function to multiply a single row of A with B using AVX for sparse-sparse multiplication
void simd_multiplyRowSparseSparse(const Matrix& A, const Matrix& B, Matrix& result, int row) {
    for (int col = 0; col < B.getCols(); ++col) {
        __m256d sum = _mm256_setzero_pd(); // Initialize sum to zero

        for (int k = 0; k < A.getCols(); k += 4) { // Increment k by 4 for AVX
            // Create temporary arrays to hold data
            double a_data[4];
            double b_data[4];

            // Load 4 elements into temporary arrays from A and B
            for (int i = 0; i < 4; ++i) {
                if (k + i < A.getCols()) {
                    a_data[i] = A.get(row, k + i); // Get values from A
                } else {
                    a_data[i] = 0.0; // Padding with zeros if out of bounds
                }
                if (k + i < B.getRows()) {
                    b_data[i] = B.get(k + i, col); // Get values from B
                } else {
                    b_data[i] = 0.0; // Padding with zeros if out of bounds
                }
            }

            // Load data into AVX registers
            __m256d a = _mm256_loadu_pd(a_data); // Load 4 elements from temporary array
            __m256d b = _mm256_loadu_pd(b_data); // Load 4 elements from temporary array
            sum = _mm256_add_pd(sum, _mm256_mul_pd(a, b)); // Multiply and accumulate
        }

        double result_arr[4];
        _mm256_storeu_pd(result_arr, sum); // Store the result
        result.set(row, col, result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3]); // Sum all results
    }
}

// Sparse-Sparse multiplication using AVX
Matrix simd_sparse_sparse_multiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        simd_multiplyRowSparseSparse(A, B, result, i);
    }

    return result;
}
