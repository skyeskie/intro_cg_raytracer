#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
    Vector3();
    Vector3(float, float, float);

    Vector3 cross(Vector3&);

    void make_unit();

    float norm();

    Vector3 operator*(float) const;
    
    float i;
    float j;
    float k;
};

Vector3 operator*(float s, const Vector3& v);

#endif // VECTOR3_H
