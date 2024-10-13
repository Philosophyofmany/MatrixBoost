// experimental_multithreading.h
#ifndef EXPERIMENTAL_MULTITHREADING_H
#define EXPERIMENTAL_MULTITHREADING_H

#include "matrix.hpp"

// Function declarations
Matrix experimentalDenseDenseMultiply(const Matrix& A, const Matrix& B);
Matrix experimentalDenseSparseMultiply(const Matrix& A, const Matrix& B);
Matrix experimentalSparseSparseMultiply(const Matrix& A, const Matrix& B);

#endif // EXPERIMENTAL_MULTITHREADING_H
