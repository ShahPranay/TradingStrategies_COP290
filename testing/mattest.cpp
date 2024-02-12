#include <iostream>
#include "Matrix.h" // Include the Matrix class declaration

int main() {
    // Create a matrix
    Matrix<int> mat1(2, 2);

    // Set values in the matrix
    mat1.set(0, 0, 1);
    mat1.set(0, 1, 2);
    mat1.set(1, 0, 3);
    mat1.set(1, 1, 4);

    // Print the original matrix
    std::cout << "Original Matrix:" << std::endl;
    mat1.print();
    std::cout << std::endl;

    // Test transpose method
    Matrix<int> transposedMat = mat1.transpose();
    std::cout << "Transposed Matrix:" << std::endl;
    transposedMat.print();
    std::cout << std::endl;

    // Test matrix multiplication
    Matrix<int> mat2(2, 2);
    mat2.set(0, 0, 1);
    mat2.set(0, 1, 0);
    mat2.set(1, 0, 0);
    mat2.set(1, 1, 1);

    Matrix<int> productMat = mat1 * mat2;
    std::cout << "Matrix Product:" << std::endl;
    productMat.print();
    std::cout << std::endl;

    // Test matrix inverse
    Matrix<double> mat3(2, 2);
    mat3.set(0, 0, 4);
    mat3.set(0, 1, 7);
    mat3.set(1, 0, 2);
    mat3.set(1, 1, 6);
    
    std::cout<<"Mat3:\n";
    mat3.print();

    Matrix<double> inverseMat = mat3.inverse();
    std::cout << "Inverse Matrix:" << std::endl;
    inverseMat.print();
    std::cout << std::endl;

    return 0;
}
