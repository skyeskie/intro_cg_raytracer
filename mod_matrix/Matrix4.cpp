#include "Matrix4.h"
using namespace std;

Matrix4::Matrix4()
{
    //Leave container uninit
}

Matrix4::Matrix4(float n)
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            data[i][j] = n;
}

Matrix4::Matrix4(float n, float d) {
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            data[i][j] = (i==j)?d:n;
}

Matrix4::Matrix4(float Ca, float Cb, float Cc, float Cd,
                 float Ce, float Cf, float Cg, float Ch,
                 float Ci, float Cj, float Ck, float Cl,
                 float Cm, float Cn, float Co, float Cp)
{
    data[0][0] = Ca; data[0][1] = Cb; data[0][2] = Cc; data[0][3] = Cd;
    data[1][0] = Ce; data[1][1] = Cf; data[1][2] = Cg; data[1][3] = Ch;
    data[2][0] = Ci; data[2][1] = Cj; data[2][2] = Ck; data[2][3] = Cl;
    data[3][0] = Cm; data[3][1] = Cn; data[3][2] = Co; data[3][3] = Cp;
}

Matrix4 Matrix4::transpose()
{
    Matrix4 t(data[0][0], data[1][0], data[2][0], data[3][0],
              data[0][1], data[1][1], data[2][1], data[3][1],
              data[0][2], data[1][2], data[2][2], data[3][2],
              data[0][3], data[1][3], data[2][3], data[3][3] );
    return t;
}


float& Matrix4::at(int x, int y)
{
    return data[x][y];
}

float Matrix4::at(int x, int y) const
{
    return data[x][y];
}

Matrix4& Matrix4::operator*=(Matrix4& m)
{
    return *this = *this * m;
}


Matrix4 Matrix4::operator*(Matrix4& rhs)
{
    Matrix4 tmp;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            tmp.data[i][j] = 0;
            for(int k=0; k<4; k++) {
                tmp.data[i][j] += data[i][k]*rhs.data[k][j];
            }
        }
    }
    
    return tmp;
}

Vector4 Matrix4::operator*(Vector4& v)
{
    Vector4 out;
    for(int i=0; i<4; i++) {
        out.at(i) = 0;
        for(int j=0; j<4; j++) {
            out.at(i) += v.at(j)*data[i][j];
        }
    }
    return out;

}

ostream& operator<<(ostream& out, Matrix4& m)
{
    for(int i=0; i<4; i++) {
        out << "   " << ((i)?" ":"[");
        for(int j=0; j<4; j++) {
            out << " " <<  m.at(i,j);
            out << ((j==3)? " " : ",");
        }
        out << ((i==3)?" ]" : "; ") << "\n";
    }
    out << "\n";
    return out;
}

//Scalar mult.

Matrix4 operator*(float s, const Matrix4& M)
{
    return M*s;
}

Matrix4 Matrix4::operator*(float s) const
{
    Matrix4 tmp(*this);
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            tmp.data[i][j] *= s;

    return tmp;
}


