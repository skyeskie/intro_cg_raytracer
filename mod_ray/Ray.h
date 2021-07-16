#ifndef RAY_H
#define RAY_H
#include "../mod_matrix/Vector3.h"
#include "../mod_geo/Shape.h"
#include "../mod_geo/RGBscale.h"

typedef RGBscale RGB;


class Shape;//Forward declaration
class Ray {
public:
    Ray();
    Ray(const Vector3& o, const Vector3& d, int refl = 0);
    Ray(const Ray& in, const Vector3& norm);

    void reset(Vector3& o, Vector3& d, int refl = 0);
    
    RGBscale get_color();
    void set_color(RGBscale);

    bool canReflect();

    const Vector3& get_origin();
    const Vector3& get_dir();

    ///Properties set by intersect methods
    ///For good OOP these should be abstracted away, but w/e
    float tnear;
    float tfar; //Also the intersect point (if intersects)
    
    Shape* intersect;
    
    //Shape params on intersect
    Vector3 intersect_params;
    Vector3 intersect_pt;

    Vector3 norm;

    int depth() { return n_refl; };
    
private:
    Vector3 origin;
    Vector3 direction;
    RGB color;

    int n_refl;
};

#endif // RAY_H
