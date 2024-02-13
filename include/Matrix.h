#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <stdexcept>

template<typename T>
class Matrix {
private:
    int _rows;
    int _cols;
    std::vector<T> _data;

public:
    // Constructor
    Matrix(int rows, int cols);

    Matrix(const Matrix& other);

    Matrix(const Matrix&& other);

    // Access and set methods with bounds checking
    const T& get(int i, int j) const;
    void set(int i, int j, T val);
    void setRow(int i, const Matrix<T> &rowdata);

    // Transpose method
    Matrix<T> transpose() const;

    // Matrix multiplication
    Matrix<T> multiply(const Matrix<T>& other) const;

    // Matrix inverse using Gaussian elimination
    Matrix<T> inverse() const;

    // Overload the * operator for multiplication
    Matrix<T> operator*(const Matrix<T>& other) const;

    void print() const;
};

template<typename T>
Matrix<T>::Matrix(int rows, int cols) : _rows(rows), _cols(cols), _data(rows * cols) {}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) : _rows(other._rows), _cols(other._cols), _data(other._data) {}

template<typename T>
Matrix<T>::Matrix(const Matrix&& other) : _rows(other._rows), _cols(other._cols), _data(other._data) {}

// Access and set methods with bounds checking
template<typename T>
const T& Matrix<T>::get(int i, int j) const {
    if (i < 0 || i >= _rows || j < 0 || j >= _cols) {
      std::cerr << "Matrix error" << std::endl;
        throw std::out_of_range("Index out of range");
    }
    return _data[i * _cols + j];
}

template<typename T>
void Matrix<T>::set(int i, int j, T val) {
    if (i < 0 || i >= _rows || j < 0 || j >= _cols) {
      std::cerr << "Matrix error" << std::endl;
        throw std::out_of_range("Index out of range");
    }
    _data[i * _cols + j] = val;
}

template<typename T>
void Matrix<T>::setRow(int i, const Matrix<T> &rowdata){
  if(i < 0 || i >= _rows || rowdata._rows != 1 || rowdata._cols != _cols){
      std::cerr << "Matrix error" << std::endl;
    throw std::out_of_range("Index out of range");
  }
  for(int j = 0; j < _cols; j++){
    _data[i * _cols + j] = rowdata.get(0, j);
  }
}

// Transpose method
template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> transposed(_cols, _rows);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            transposed.set(j, i, get(i, j));
        }
    }
    return transposed;
}

// Matrix multiplication
template<typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T>& other) const {
    if (_cols != other._rows) {
      std::cerr << "Matrix error" << std::endl;
        throw std::invalid_argument("Matrices are not multipliable");
    }

    Matrix<T> result(_rows, other._cols);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < other._cols; ++j) {
            T sum = 0;
            for (int k = 0; k < _cols; ++k) {
                sum += get(i, k) * other.get(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

// Matrix inverse using Gaussian elimination
template<typename T>
Matrix<T> Matrix<T>::inverse() const {
    if (_rows != _cols) {
      std::cerr << "Matrix error" << std::endl;
      throw std::invalid_argument("Matrix is not square");
    }

    // Augmenting the original matrix with an identity matrix
    Matrix<T> augmented(_rows, _cols * 2);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            augmented.set(i, j, get(i, j));
        }
        augmented.set(i, i + _cols, static_cast<T>(1));
    }

    // Perform row operations to get the RREF
    for (int i = 0; i < _rows; ++i) {
        // Scaling the current row to make the pivot element 1
        T pivot = augmented.get(i, i);
        if (pivot == 0) {
      std::cerr << "Matrix error" << std::endl;
            throw std::runtime_error("Matrix is not invertible");
        }
        for (int j = i; j < _cols * 2; ++j) {
            augmented.set(i, j, augmented.get(i, j) / pivot);
        }

        // Eliminating other elements in the current column
        for (int k = 0; k < _rows; ++k) {
            if (k != i) {
                T factor = augmented.get(k, i);
                for (int j = i; j < _cols * 2; ++j) {
                    augmented.set(k, j, augmented.get(k, j) - factor * augmented.get(i, j));
                }
            }
        }
    }

    // Extracting the inverse matrix from the augmented matrix
    Matrix<T> inverseMatrix(_rows, _cols);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            inverseMatrix.set(i, j, augmented.get(i, j + _cols));
        }
    }

    return inverseMatrix;
}

// Overload the * operator for multiplication
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    return multiply(other);
}

template<typename T>
void Matrix<T>::print() const {
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      std::cout << get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}

#endif // MATRIX_H
