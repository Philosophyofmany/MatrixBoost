#include "experimental_multithreading.hpp"
#include <thread>
#include <vector>
#include <iostream>

// Function to prompt the user for the number of threads
int getUserThreadCount() {
    int numThreads;
    std::cout << "Enter the number of threads to use: ";
    std::cin >> numThreads;
    return numThreads;
}

// Function to multiply a block of rows of A with B (for dense-dense)
void experimentalMultiplyRowBlock(const Matrix& A, const Matrix& B, Matrix& result, int startRow, int endRow) {
    for (int row = startRow; row < endRow; ++row) {
        for (int col = 0; col < B.getCols(); ++col) {
            double sum = 0.0;
            for (int k = 0; k < A.getCols(); ++k) {
                sum += A.get(row, k) * B.get(k, col);
            }
            result.set(row, col, sum);
        }
    }
}

// Experimental Mode: Multithreaded multiplication for Dense-Dense
Matrix experimentalDenseDenseMultiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    int numThreads = getUserThreadCount();
    std::vector<std::thread> threads;

    // Calculate the block size per thread
    int blockSize = rows / numThreads;
    int remainder = rows % numThreads;

    int startRow = 0;
    for (int i = 0; i < numThreads; ++i) {
        int endRow = startRow + blockSize;
        if (i < remainder) {  // Distribute remainder rows
            endRow++;
        }
        threads.emplace_back(experimentalMultiplyRowBlock, std::ref(A), std::ref(B), std::ref(result), startRow, endRow);
        startRow = endRow;  // Move to the next block of rows
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    return result;
}

// Function to multiply a block of rows of A with B (for dense-sparse)
void experimentalMultiplyRowBlockSparse(const Matrix& A, const Matrix& B, Matrix& result, int startRow, int endRow) {
    for (int row = startRow; row < endRow; ++row) {
        for (int col = 0; col < B.getCols(); ++col) {
            double sum = 0.0;
            for (int k = 0; k < A.getCols(); ++k) {
                sum += A.get(row, k) * B.get(k, col);
            }
            result.set(row, col, sum);
        }
    }
}

// Experimental Mode: Dense-Sparse multiplication
Matrix experimentalDenseSparseMultiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    int numThreads = getUserThreadCount();
    std::vector<std::thread> threads;

    int blockSize = rows / numThreads;
    int remainder = rows % numThreads;

    int startRow = 0;
    for (int i = 0; i < numThreads; ++i) {
        int endRow = startRow + blockSize;
        if (i < remainder) {
            endRow++;
        }
        threads.emplace_back(experimentalMultiplyRowBlockSparse, std::ref(A), std::ref(B), std::ref(result), startRow, endRow);
        startRow = endRow;
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}

// Function to multiply a block of rows of A with B (for sparse-sparse)
void experimentalMultiplyRowBlockSparseSparse(const Matrix& A, const Matrix& B, Matrix& result, int startRow, int endRow) {
    for (int row = startRow; row < endRow; ++row) {
        for (int col = 0; col < B.getCols(); ++col) {
            double sum = 0.0;
            for (int k = 0; k < A.getCols(); ++k) {
                if (A.get(row, k) != 0 && B.get(k, col) != 0) {  // Only compute if both are non-zero
                    sum += A.get(row, k) * B.get(k, col);
                }
            }
            result.set(row, col, sum);
        }
    }
}

// Experimental Mode: Sparse-Sparse multiplication
Matrix experimentalSparseSparseMultiply(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);

    int numThreads = getUserThreadCount();
    std::vector<std::thread> threads;

    int blockSize = rows / numThreads;
    int remainder = rows % numThreads;

    int startRow = 0;
    for (int i = 0; i < numThreads; ++i) {
        int endRow = startRow + blockSize;
        if (i < remainder) {
            endRow++;
        }
        threads.emplace_back(experimentalMultiplyRowBlockSparseSparse, std::ref(A), std::ref(B), std::ref(result), startRow, endRow);
        startRow = endRow;
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}
