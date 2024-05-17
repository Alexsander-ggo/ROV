#include <iostream>

#include "Matrix.h"

void SimpleText(const Matrix& A, const Vector& B) {
    try {
        Vector X = SLAE(A, B);
        std::cout << "Matrix A:" << std::endl;
        Print(A);
        std::cout << "Matrix B:" << std::endl;
        Print({B});
        if (X.empty()) {
            throw 0;
        }
        std::cout << "Matrix X:" << std::endl;
        Print({X});
        Matrix invA = Inverse(A);
        std::cout << "Matrix invA:" << std::endl;
        Print(invA);
        std::cout << "Mult A and invA:" << std::endl;
        Print(Mult(A, invA));
        std::cout << "Mult A and X:" << std::endl;
        Print(Mult(A, Transpose({X})));
    } catch (...) {
        std::cout << "Not inverse matrix A and solution X\n" << std::endl;
    }
}

void Test() {
    {
        std::cout << "TEST 1" << std::endl;
        Matrix A = {{1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};
        Vector B = {1, 2, 3, 4};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 2" << std::endl;
        Matrix A = {{1, 0, 0, 0},
                    {0, 2, 0, 0},
                    {0, 0, 3, 0},
                    {0, 0, 0, 4}};
        Vector B = {1, 2, 3, 4};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 3" << std::endl;
        Matrix A = {{0, 0, 0, 1},
                    {0, 0, 2, 0},
                    {0, 3, 0, 0},
                    {4, 0, 0, 0}};
        Vector B = {1, 1, 1, 1};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 4" << std::endl;
        Matrix A = {{1, 2, 0, 0},
                    {2, 4, 3, 0},
                    {0, 2, 0, 0},
                    {0, 0, 0, 4}};
        Vector B = {1, 1, 1, 1};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 5" << std::endl;
        Matrix A = {{1, 2, 3, 4},
                    {5, 6, 7, 8},
                    {9, 10, 11, 12},
                    {16, 15, 14, 13}};
        Vector B = {1, 1, 1, 1};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 6" << std::endl;
        Matrix A = {{1, 2, 3, 4},
                    {-1, -2, 0, 3},
                    {0, 6, 11, 2},
                    {4, -2, 1, 0}};
        Vector B = {5, -2, 14, 4};
        SimpleText(A, B);
    }
    {
        std::cout << "TEST 7" << std::endl;
        Matrix A = {{10, 6, 2, 0},
                    {5, 1, -2, 4},
                    {3, 5, 1, -1},
                    {0, 6, -2, 2}};
        Vector B = {25, 14, 10, 8};
        SimpleText(A, B);
    }
}

int main()
{
    Test();
    return 0;
}
