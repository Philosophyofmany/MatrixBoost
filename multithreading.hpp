#ifndef MULTITHREADING_HPP
#define MULTITHREADING_HPP

#include "matrix.hpp"

// Function declarations
Matrix denseDenseMultiplyThreaded(const Matrix& A, const Matrix& B);
Matrix denseSparseMultiplyThreaded(const Matrix& A, const Matrix& B);
Matrix sparseSparseMultiplyThreaded(const Matrix& A, const Matrix& B);

#endif // MULTITHREADING_HPP
