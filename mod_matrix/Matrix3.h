#ifndef MATRIX3_H
#define MATRIX3_H
#include "Vector3.h"
#include <iostream>

class Matrix3  {
public:
    Matrix3();

    Matrix3(float);

    //Creates matrix with diagonal different
    Matrix3(float, float);

    Matrix3(float a, float b, float c,
            float d, float e, float f,
            float g, float h, float i);

    Matrix3(Vector3&, Vector3&, Vector3& );
    
    Matrix3 operator*( Matrix3& );
    Matrix3& operator*=( Matrix3& );
    Vector3 operator*( Vector3& );
    Matrix3 operator*( float ) const;

    Matrix3 transpose();

    Matrix3 col_replace(int, Vector3& );

    float det();

    float& at(int, int);
    float at(int, int) const;

private:
    float data[3][3];
};

std::ostream& operator<<(std::ostream&, Matrix3& );

Matrix3 operator*(float, const Matrix3& );

#endif // MATRIX3_H
