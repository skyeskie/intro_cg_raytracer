#include "Vector3.h"
#include <math.h>
#include <stdexcept>

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

//Dot product
float Vector3::operator*(const Vector3& rhs) const {
    return i*rhs.i + j*rhs.j + k*rhs.k;
}

//Other mult:
Vector3 Vector3::operator+(const Vector3& rhs) const {
    Vector3 tmp(*this);
    tmp.i += rhs.i;
    tmp.j += rhs.j;
    tmp.k += rhs.k;
    return tmp;
}

Vector3& Vector3::operator+=(const Vector3& rhs) {
    i += rhs.i;
    j += rhs.j;
    k += rhs.k;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
    Vector3 tmp(*this);
    tmp.i -= rhs.i;
    tmp.j -= rhs.j;
    tmp.k -= rhs.k;
    return tmp;
}

Vector3 Vector3::operator-(void )
{
    Vector3 tmp(*this);
    tmp.i *= -1;
    tmp.j *= -1;
    tmp.k *= -1;
    return tmp;
}

float& Vector3::at(int x)
{
    if(x==0) return i;
    if(x==1) return j;
    if(x==2) return k;
    throw std::overflow_error("Tried to access out of Vector range.");
    return i;
}

float Vector3::at(int x) const
{
    if(x==0) return i;
    if(x==1) return j;
    if(x==2) return k;
    throw std::overflow_error("Tried to access out of Vector range.");
    return -1;
}

std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    out << "( " << v.i << ", " << v.j << ", " << v.k << " )";
    return out;
}
