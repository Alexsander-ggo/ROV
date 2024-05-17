#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>
#include <iostream>
#include <vector>

using Type = double;

using Size = uint64_t;

using Vector = std::vector<Type>;

using Matrix = std::vector<Vector>;

void Print(const Matrix& matrix);

Matrix Mult(const Matrix& left, const Matrix& right);

Matrix Transpose(const Matrix& matrix);

Matrix Inverse(const Matrix& matrix);

Vector SLAE(const Matrix& system, const Vector& result);

#endif // MATRIX_H
