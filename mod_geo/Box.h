#ifndef BOX_H
#define BOX_H
#include "../mod_matrix/Vector3.h"
#include "RGBscale.h"
#include "../mod_ray/Ray.h"
#include "Shape.h"

typedef RGBscale RGB;
typedef Vector3 V3;

class Box : public Shape
{
public:
    Box(V3 x1, V3 x2, RGB amb, RGB refl, RGB spec);

    virtual bool intersect(Ray& );
    
    virtual Ray reflect(const Ray&);
    
    virtual void set_norm(Ray&);
    
    virtual Vector3 get_norm(const Vector3&);
    
private:
    Vector3 llcorner;
    Vector3 urcorner;
};

#endif