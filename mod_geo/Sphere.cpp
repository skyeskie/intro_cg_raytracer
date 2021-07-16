#include "Sphere.h"
#include <cmath>

Sphere::Sphere(Vector3 center, float radius, RGB amb, RGB refl, RGB spec)
    :Shape(amb, refl, spec), c(center), r(radius)
{}

bool Sphere::intersect(Ray& pt)
{
    Vector3 e = pt.get_origin(),
            d = pt.get_dir(),
            emc = e - c,
            p, norm;
    //Check discrim
    float disc = (d*emc)*(d*emc)-(d*d)*(emc*emc-r*r);
        
    if(disc>0) {//Ignore tangents for now
        disc = sqrt(disc);
        float t = (-d*emc - disc)/(d*d);
        if(t < pt.tfar && t > pt.tnear) {
            pt.tfar = t;
            pt.intersect = this;
            pt.intersect_pt = e + t*d;
            set_norm(pt);
        }
        //Check other case
        t = (-d*emc + disc)/(d*d);
        if(t < pt.tfar && t > pt.tnear) {
            pt.tfar = t;
            pt.intersect = this;
            pt.intersect_pt = e + t*d;
            set_norm(pt);
        }
    }
    return false;
}

Ray Sphere::reflect(const Ray& r)
{
    return Ray(r, r.norm);
}

void Sphere::set_norm(Ray& r) {
    r.norm = r.intersect_pt - c;
    r.norm.make_unit();
}

Vector3 Sphere::get_norm(const Vector3& p)
{
    Vector3 tmp(p - c);
    return tmp;
}


