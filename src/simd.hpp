#ifndef SIMD_HPP
#define SIMD_HPP

#include "matrix.hpp"

// Function to perform dense-dense matrix multiplication using SIMD
Matrix simd_dense_dense_multiply(const Matrix& A, const Matrix& B);

// Function to perform dense-sparse matrix multiplication using SIMD
Matrix simd_dense_sparse_multiply(const Matrix& A, const Matrix& B);

// Function to perform sparse-sparse matrix multiplication using SIMD
Matrix simd_sparse_sparse_multiply(const Matrix& A, const Matrix& B);

#endif // SIMD_HPP
