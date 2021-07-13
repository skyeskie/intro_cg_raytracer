#ifndef VECTOR4_H
#define VECTOR4_H
#include <iostream>

class Vector4
{
public:
    Vector4();

    Vector4(float, float, float, float);
    
    float& at(int);
    float at(int) const;

    float& operator[](int pos);
    float operator[](int pos) const;

private:
    float data[4];
};

std::ostream& operator<<(std::ostream& , Vector4& );

#endif // VECTOR4_H
