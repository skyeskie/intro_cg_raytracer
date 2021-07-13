#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

template < typename T >
class Matrix
{
public:
    Matrix();
    Matrix(int, int);
    Matrix(const Matrix<T>& rhs);
    ~Matrix();
    Matrix<T> operator=(const Matrix<T>& rhs);

    
    Matrix<T>& operator+=(const Matrix<T>& m);
    Matrix<T> operator+(const Matrix<T>& m) const;
    Matrix<T>& operator-();
    Matrix<T>& operator-(Matrix<T>& m);
    Matrix<T>& operator*(Matrix<T>& m);
    Matrix<T>& operator==(Matrix<T>& m);
    Matrix<T>& operator*(float s);

    template< typename Q >
    friend std::ostream& operator<<(std::ostream& out, Matrix<Q>& m );

    float& at(int m, int n) const;
    
private:
    float& at_(int m, int n) const;
    
    int width;
    int length;

    T* matrix_;
};

//Handle other order for scalar multiplication, s*M
template< typename T >
Matrix<T>& operator*(float s, Matrix<T>& m);

//Output
template< typename T >
std::ostream& operator<< (std::ostream& out, const Matrix<T>& m);


#include "Matrix.cpp"
#endif // MATRIX_H
