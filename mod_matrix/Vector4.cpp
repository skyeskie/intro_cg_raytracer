#include "Vector4.h"
using namespace std;

Vector4::Vector4()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
}

Vector4::Vector4(float a, float b, float c, float d)
{
    data[0] = a;
    data[1] = b;
    data[2] = c;
    data[3] = d;
}

float& Vector4::at(int x)
{
    return data[x];
}

float Vector4::at(int x) const
{
    return data[x];
}


ostream& operator<<(ostream& ofs, Vector4& v)
{

    ofs<< "[ ";
    for(int i=0; i<4; i++)
        ofs << v.at(i) << ((i==3)?" ]" : ", ");

    return ofs;
}

float& Vector4::operator[](int pos)
{
    return data[pos];
}

float Vector4::operator[](int pos) const
{
    return data[pos];
}


