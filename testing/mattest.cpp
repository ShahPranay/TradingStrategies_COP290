#include <iostream>
#include "Matrix.h" // Include the Matrix class declaration

int main() {
    // Create a matrix
    // Test matrix inverse
    Matrix<double> x(3, 3);
    x.set(0, 0, 4);
    x.set(0, 1, 7);
    x.set(0, 2, 9);
    x.set(1, 0, 2);
    x.set(1, 1, 6);
    x.set(1, 2, 5);
    x.set(2, 0, 4);
    x.set(2, 1, 9);
    x.set(2, 2, 11);
    
    std::cout<<"x:\n";
    x.print();

    Matrix<double> xt = x.transpose();
    
    std::cout<<"transpose\n";
    x.print();

    std::cout<<"xt * x\n";
    (xt * x).print();

    std::cout<<"(xt * x)^-1 * xt\n";
    ((xt * x).inverse() * xt).print();

    return 0;
}
