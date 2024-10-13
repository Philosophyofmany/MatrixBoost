#include "multithreading.hpp"
#include <thread>
#include <vector>

// Function to multiply a single row of A with B
void multiplyRow(const Matrix& A, const Matrix& B, Matrix& result, int row) {
    for (int col = 0; col < B.getCols(); ++col) {
        double sum = 0.0;
        for (int k = 0; k < A.getCols(); ++k) {
            sum += A.get(row, k) * B.get(k, col);
        }
        result.set(row, col, sum);
    }
    //debug statement
    // std::cout << "Thread " << std::this_thread::get_id() << " processing row " << row << std::endl;
}

// Dense-Dense multiplication with multithreading
Matrix denseDenseMultiplyThreaded(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);
    std::vector<std::thread> threads;

    for (int i = 0; i < rows; ++i) {
        threads.emplace_back(multiplyRow, std::ref(A), std::ref(B), std::ref(result), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}

// Function to multiply a single row of A with a column of B (for sparse)
void multiplyRowSparse(const Matrix& A, const Matrix& B, Matrix& result, int row) {
    for (int col = 0; col < B.getCols(); ++col) {
        double sum = 0.0;
        for (int k = 0; k < A.getCols(); ++k) {
            sum += A.get(row, k) * B.get(k, col);
        }
        result.set(row, col, sum);
    }
}

// Dense-Sparse multiplication with multithreading
Matrix denseSparseMultiplyThreaded(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);
    std::vector<std::thread> threads;

    for (int i = 0; i < rows; ++i) {
        threads.emplace_back(multiplyRowSparse, std::ref(A), std::ref(B), std::ref(result), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}

// Function to multiply a single row of A with a column of B (for sparse-sparse)
void multiplyRowSparseSparse(const Matrix& A, const Matrix& B, Matrix& result, int row) {
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

// Sparse-Sparse multiplication with multithreading
Matrix sparseSparseMultiplyThreaded(const Matrix& A, const Matrix& B) {
    int rows = A.getRows();
    int cols = B.getCols();
    Matrix result(rows, cols);
    std::vector<std::thread> threads;

    for (int i = 0; i < rows; ++i) {
        threads.emplace_back(multiplyRowSparseSparse, std::ref(A), std::ref(B), std::ref(result), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}
