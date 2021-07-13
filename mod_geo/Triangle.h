#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "../mod_matrix/Vector3.h"
#include "RGBscale.h"
#include "../mod_ray/Ray.h"
#include "Shape.h"

typedef RGBscale RGB;
typedef Vector3 V3;

class Triangle : public Shape
{
public:
    Triangle(V3 x, V3 y, V3 z, RGB amb, RGB refl, RGB spec);

    virtual bool intersect(Ray& pt);
    
    virtual Ray reflect(const Ray& );
    
    virtual void set_norm(Ray& r);
    
    virtual Vector3 get_norm(const Vector3&);
    
private:
    V3 v1;
    V3 v2;
    V3 v3;
};

#endif