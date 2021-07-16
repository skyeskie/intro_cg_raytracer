#include "Matrix3.h"
using namespace std;

Matrix3::Matrix3()
{
    //Leave container uninit
}

Matrix3::Matrix3(float n)
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            data[i][j] = n;
}

Matrix3::Matrix3(float n, float d) {
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            data[i][j] = (i==j)?d:n;
}

Matrix3::Matrix3(Vector3& a, Vector3& b, Vector3& c)
{
    data[0][0] = a.i; data[0][1] = b.i; data[0][2] = c.i;
    data[1][0] = a.j; data[1][1] = b.j; data[1][2] = c.j;
    data[2][0] = a.k; data[2][1] = b.k; data[2][2] = c.k;
}


Matrix3::Matrix3(float Ca, float Cb, float Cc,
                 float Ce, float Cf, float Cg,
                 float Ci, float Cj, float Ck)
{
    data[0][0] = Ca; data[0][1] = Cb; data[0][2] = Cc;
    data[1][0] = Ce; data[1][1] = Cf; data[1][2] = Cg;
    data[2][0] = Ci; data[2][1] = Cj; data[2][2] = Ck;
}

Matrix3 Matrix3::transpose()
{
    Matrix3 t(data[0][0], data[1][0], data[2][0],
              data[0][1], data[1][1], data[2][1],
              data[0][2], data[1][2], data[2][2] );
    return t;
}

Matrix3 Matrix3::col_replace(int c, Vector3& v)
{
    Matrix3 tmp(*this);
    tmp.data[0][c] = v.i;
    tmp.data[1][c] = v.j;
    tmp.data[2][c] = v.k;
    return tmp;
}

float Matrix3::det()
{
    return data[0][0]*data[1][1]*data[2][2] - data[0][0]*data[2][1]*data[1][2]
         - data[0][1]*data[1][0]*data[2][2] + data[0][1]*data[2][0]*data[1][2]
         + data[0][2]*data[1][0]*data[2][1] - data[0][2]*data[2][0]*data[1][1];
}


float& Matrix3::at(int x, int y)
{
    return data[x][y];
}

float Matrix3::at(int x, int y) const
{
    return data[x][y];
}

Matrix3& Matrix3::operator*=(Matrix3& m)
{
    return *this = *this * m;
}


Matrix3 Matrix3::operator*(Matrix3& rhs)
{
    Matrix3 tmp;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            tmp.data[i][j] = 0;
            for(int k=0; k<3; k++) {
                tmp.data[i][j] += data[i][k]*rhs.data[k][j];
            }
        }
    }
    
    return tmp;
}

Vector3 Matrix3::operator*(Vector3& v)
{
    Vector3 out;
    for(int i=0; i<3; i++) {
        out.at(i) = 0;
        for(int j=0; j<3; j++) {
            out.at(i) += v.at(j)*data[i][j];
        }
    }
    return out;

}

ostream& operator<<(ostream& out, Matrix3& m)
{
    for(int i=0; i<3; i++) {
        out << "   " << ((i)?" ":"[");
        for(int j=0; j<3; j++) {
            out << " " <<  m.at(i,j);
            out << ((j==3)? " " : ",");
        }
        out << ((i==3)?" ]" : "; ") << "\n";
    }
    out << "\n";
    return out;
}

//Scalar mult.

Matrix3 operator*(float s, const Matrix3& M)
{
    return M*s;
}

Matrix3 Matrix3::operator*(float s) const
{
    Matrix3 tmp(*this);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            tmp.data[i][j] *= s;

    return tmp;
}


