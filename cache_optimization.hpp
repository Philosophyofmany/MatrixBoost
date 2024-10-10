#ifndef CACHE_OPTIMIZATION_H
#define CACHE_OPTIMIZATION_H

#include "matrix.hpp"

// Function declarations
Matrix cache_optimized_multiply_dense_dense(const Matrix& A, const Matrix& B);
Matrix cache_optimized_multiply_dense_sparse(const Matrix& A, const Matrix& B);
Matrix cache_optimized_multiply_sparse_sparse(const Matrix& A, const Matrix& B);

#endif // CACHE_OPTIMIZATION_H
