#ifndef MATRIX4_H
#define MATRIX4_H
#include "Vector4.h"
#include <iostream>

class Matrix4  {
public:
    Matrix4();

    Matrix4(float);

    //Creates matrix with diagonal different
    Matrix4(float, float);

    Matrix4(float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float m, float n, float o, float p);
            
    Matrix4 operator*( Matrix4& );
    Matrix4& operator*=( Matrix4& );
    Vector4 operator*( Vector4& );
    Matrix4 operator*( float ) const;

    Matrix4 transpose();

    float& at(int, int);
    float at(int, int) const;

private:
    float data[4][4];
};

std::ostream& operator<<(std::ostream&, Matrix4& );

Matrix4 operator*(float, const Matrix4& );

#endif // MATRIX4_H
