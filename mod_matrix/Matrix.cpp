#include "Matrix.h"
#include <stdexcept>
#include <algorithm>
using namespace std;

template < typename T >
Matrix<T>::Matrix()
    :width(0), length(0), matrix_(NULL)
{}

template < typename T >
Matrix<T>::Matrix(int m, int n)
    :width(m), length(n), matrix_(new T[m*n])
{}


template < typename T >
Matrix<T>::Matrix(const Matrix<T>& rhs )
    :width(rhs.width), length(rhs.length),
    matrix_(new T[rhs.length*rhs.width])
{
    copy(rhs.matrix_, rhs.matrix_+length*width, matrix_);
}

template < typename T >
Matrix< T > Matrix<T>::operator=(const Matrix< T >& rhs )
{
    if(this == &rhs) return *this;

    Matrix<T> tmp(rhs);
    swap(tmp.matrix_, matrix_);
    swap(tmp.width, width);
    swap(tmp.length, length);

    return *this;
}

template < typename T >
Matrix<T>::~Matrix()
{
    delete matrix_;
}


template < typename T >
Matrix< T >& Matrix<T>::operator==( Matrix< T >& m )
{
    return width == m.width && length == m.length &&
        equal(matrix_, matrix_ + width*length, m.matrix_);
}


template < typename T >
float& Matrix<T>::at( int m, int n ) const
{
    if(m<0 || m>= width || n<0 || n>= length) {
        throw out_of_range("Tried to access out of matrix bounds");
    }
    return at_(m,n);
}

//No bounds checking
template < typename T >
inline float& Matrix<T>::at_( int m, int n ) const
{
    return *(matrix_ + m + n*width);
}

template < typename T >
Matrix< T >& Matrix<T>::operator+=(const Matrix< T >& m )
{
    if(m.length != length || m.width != width)
        throw invalid_argument("Matrix dimensions must match.");

    for(int i=0; i<width*length; i++) {
        *(matrix_+i) += *(m.matrix_+i);
    }
    return *this;
}

template < typename T >
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) const
{
    if(m.length != length || m.width != width)
        throw invalid_argument("Matrix dimensions must match.");

    Matrix<T> tmp(*this);
    tmp += m;

    return tmp;
}

template < typename T >
Matrix< T >& Matrix<T>::operator-( Matrix< T >& m )
{
    return (*this) + (m*-1.0); //Force scalar mult and add
}

template < typename T >
Matrix< T >& Matrix<T>::operator-()
{
    return (*this)*(-1.0);
}


//Matrix mult
template < typename T >
Matrix< T >& Matrix<T>::operator*( Matrix< T >& m ) {
    //Todo
    return *this;
}

//Scalar mult
template < typename T >
Matrix<T>& Matrix<T>::operator*( float s ) {
    for(T* i = matrix_; i<matrix_ +length*width; i++) {
        *i = s * (*i);
    }
}

template< typename T >
Matrix< T >& operator*( float s, Matrix< T >& m )
{
    return m*s; //Use operator* within class
}

template< typename T >
ostream& operator<<( ostream& out, Matrix< T >& m )
{
    for(int i=0; i<m.length; i++) {
        out << "   " << ((i)?" ":"[");
        for(int j=0; j<m.width; j++) {
            out << " " <<  m.at(j,i);
            out << ((j==m.width-1)? " " : ",");
        }
        cout << ((i==m.length-1)?" ]" : "; ") << endl;
    }
    
    return out;
}
