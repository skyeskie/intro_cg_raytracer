#ifndef SHAPE_H
#define SHAPE_H
#include "../mod_ray/Ray.h"
#include "RGBscale.h"

typedef RGBscale RGB;
class Ray; //Forward declaration

class Shape
{
public:
    Shape(RGB a, RGB r, RGB s)
        :c_ambient(a), c_reflect(r), c_specular(s) {};
    
    /**Intersection calc
     * determines if ray hits object
     * @return True if ray hits object
     * @post if ray hits, updates ray
     */
    virtual bool intersect(Ray& ) = 0;

    /**Reflect a ray for refl. calc
     * @arg ray that intersects object
     * @return reflection of ray
     */
    virtual Ray reflect(const Ray&) = 0;

    virtual void set_norm(Ray&) = 0;

    virtual Vector3 get_norm(const Vector3&) = 0;

    //Get colors for light calc
    virtual const RGBscale& get_ambient_color()
        { return c_ambient; };
    virtual const RGBscale& get_specular_color()
        { return c_specular; };
    virtual const RGBscale& get_reflect_color()
        { return c_reflect; };


private:
    RGB c_ambient;
    RGB c_reflect;
    RGB c_specular;
};

#endif // SHAPE_H
