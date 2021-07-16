#ifndef VECTOR3_H
#define VECTOR3_H
#include <iostream>

class Vector3
{
public:
    Vector3();
    Vector3(float, float, float);

    Vector3 cross(Vector3&);

    void make_unit();

    float norm();

    Vector3 operator*(float) const;
    float operator*(const Vector3& ) const;

    Vector3 operator+(const Vector3& ) const;
    Vector3& operator+=(const Vector3& );

    Vector3 operator-(const Vector3& ) const;

    Vector3 operator-(void);

    float& at(int);
    float at(int) const;
    
    float i;
    float j;
    float k;
};

Vector3 operator*(float s, const Vector3& v);
std::ostream& operator<<(std::ostream& out, const Vector3& v);
#endif // VECTOR3_H
