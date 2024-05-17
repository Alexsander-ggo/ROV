#include "Matrix.h"

const Type Eps = 0.000001;

void Print(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Matrix Mult(const Matrix& left, const Matrix& right)
{
    Size leftCols = left.front().size();
    Size rightRows = right.size();
    if (leftCols != rightRows) {
        return {};
    }
    Size leftRows = left.size();
    Size rightCols = right.front().size();
    Matrix mult(leftRows, Vector(rightCols, 0));
    for (Size i = 0; i < leftRows; ++i) {
        for (Size j = 0; j < rightCols; ++j) {
            for (Size k = 0; k < leftCols; ++k) {
                mult[i][j] += left[i][k] * right[k][j];
            }
        }
    }
    return mult;
}

Matrix Transpose(const Matrix& matrix)
{
    if (matrix.empty()) {
        return {};
    }
    Matrix transpose(matrix.front().size(), Vector(matrix.size()));
    Size rows = transpose.size();
    Size cols = transpose.front().size();
    for (Size i = 0; i < rows; ++i) {
        for (Size j = 0; j < cols; ++j) {
            transpose[i][j] = matrix[j][i];
        }
    }
    return transpose;
}

Matrix Inverse(const Matrix& matrix)
{
    if (matrix.empty()) {
        return {};
    }
    Size size = matrix.size();
    if (size != matrix.front().size()) {
        return {};
    }
    Matrix copy(matrix);
    Matrix inverse(copy);
    for (Size i = 0; i < size; ++i) {
        for (Size j = 0; j < size; ++j) {
            inverse[i][j] = (i == j);
        }
    }
    for (Size i = 0; i < size; ++i) {
        if (std::abs(copy[i][i]) < Eps) {
            for (size_t j = i + 1; j < size; ++j) {
                if (copy[j][i] != 0) {
                    for (Size k = 0; k < size; ++k) {
                        copy[i][k] += copy[j][k];
                        inverse[i][k] += inverse[j][k];
                    }
                    break;
                }
                if (j == size - 1) {
                    return {};
                }
            }
        }
        for (size_t j = i + 1; j < size; ++j) {
            Type temp = copy[j][i] / copy[i][i];
            for (Size k = 0; k < size; ++k) {
                copy[j][k] -= temp * copy[i][k];
                inverse[j][k] -= temp * inverse[i][k];
            }
        }
    }
    for (Size i = size - 1; true; --i) {
        for (Size j = 0; j < i; ++j) {
            Type temp = copy[j][i] / copy[i][i];
            for (Size k = 0; k < size; ++k) {
                inverse[j][k] -= temp * inverse[i][k];
            }
        }
        for (Size k = 0; k < size; ++k) {
            inverse[i][k] /= copy[i][i];
        }
        if (i == 0) {
            break;
        }
    }
    return inverse;
}

Vector SLAE(const Matrix& system, const Vector& result)
{
    Matrix inverseSystem = Inverse(system);
    if (inverseSystem.empty()) {
        return {};
    }
    return Transpose(Mult(inverseSystem, Transpose({result}))).front();
}
