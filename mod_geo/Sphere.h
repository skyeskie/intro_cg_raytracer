#ifndef SPHERE_H
#define SPHERE_H
#include "../mod_matrix/Vector3.h"
#include "RGBscale.h"
#include "../mod_ray/Ray.h"
#include "Shape.h"

typedef RGBscale RGB;

class Sphere: public Shape
{
public:
    Sphere(Vector3 center, float radius, RGB amb, RGB refl, RGB spec);

    virtual bool intersect(Ray& pt);

    virtual Ray reflect(const Ray& );

    virtual void set_norm(Ray& r);

    virtual Vector3 get_norm(const Vector3&);
    
private:
    Vector3 c;
    float r;
};

#endif // SPHERE_H
