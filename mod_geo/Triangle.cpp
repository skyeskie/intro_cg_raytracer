#include "Triangle.h"
#include "../mod_matrix/Matrix3.h"
#include "../mod_matrix/Vector3.h"

Triangle::Triangle(V3 x, V3 y, V3 z, RGB amb, RGB refl, RGB spec)
    :Shape(amb, refl, spec), v1(x), v2(y), v3(z)
{}

void Triangle::set_norm(Ray& r)
{
    r.norm = v2 - v1;
    Vector3 v = v3-v1;
    r.norm = r.norm.cross(v);
    r.norm.make_unit();
}

Vector3 Triangle::get_norm(const Vector3& c)
{
    Vector3 v = v2 - v1,
            u = v3-v1;
    v.cross(u);
    return v;
}


bool Triangle::intersect(Ray& pt) {
    Vector3 e = pt.get_origin(),
            d = pt.get_dir(),
            b = v1 - e,

            ea = v1-v2,
            eb = v1-v3;

    Matrix3 A(ea, eb, d);
    float detA = A.det();

    float u = A.col_replace(0,b).det() / detA;
    if(u<0 || u>1)
        return false; //Miss

    float v = A.col_replace(1,b).det() / detA;
    if(v<0 || u+v >= 1)
        return false; //Miss

    float t = A.col_replace(2,b).det() / detA;
    if(t>=pt.tfar || t<pt.tnear)
        return false; //Not in range

    //Hit
    pt.tfar = t;
    pt.intersect = this;
    pt.intersect_pt = e + t*d;
    pt.intersect_params.i = u;
    pt.intersect_params.j = v;
    set_norm(pt);

    //Make norm always point towards ray (maybe)
    if(pt.norm * pt.get_dir() > 0) pt.norm = -pt.norm;
    return true;
}

Ray Triangle::reflect(const Ray& r) {
    return Ray(r, r.norm);
}


