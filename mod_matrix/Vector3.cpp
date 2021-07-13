#include "Vector3.h"
#include <math.h>

Vector3::Vector3()
    :i(0), j(0), k(0)
{}

Vector3::Vector3(float x, float y, float z)
    :i(x), j(y), k(z)
{}

void Vector3::make_unit()
{
    float len;
    len = sqrt(i*i + j*j + k*k);
    
    i = i/len;
    j = j/len;
    k = k/len;
}

float Vector3::norm() {
    return sqrt(i*i+j*j+k*k);
}


Vector3 Vector3::cross(Vector3& r)
{
    Vector3 z( j*r.k - k*r.j,
               k*r.i - i*r.k,
               i*r.j - j*r.i);
    return z;
}

//Scalar mult
Vector3 operator*(float s, const Vector3& v)
{
    return v*s;
}

Vector3 Vector3::operator*(float s) const
{
    Vector3 tmp(*this);
    tmp.i *=s;
    tmp.j *=s;
    tmp.k *=s;
    return tmp;
}



