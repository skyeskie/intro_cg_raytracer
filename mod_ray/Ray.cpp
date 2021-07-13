#include "Ray.h"
#include "../mod_geo/Shape.h"
#include <limits>
#include <stdlib.h>
using namespace std;

const float MIN_TRAVEL = 0.;

const float INFIN = numeric_limits<float>::infinity();

Ray::Ray()
    :tnear(MIN_TRAVEL), tfar(INFIN), intersect(NULL),
    origin(0,0,0), direction(0,0,0), color(),    
    n_refl(0)
{}

Ray::Ray(const Vector3& o, const Vector3& d, int refl)
    :tnear(MIN_TRAVEL), tfar(INFIN), intersect(NULL),
    origin(o), direction(d), color(),    
    n_refl(refl)
{}

void Ray::reset(Vector3& o, Vector3& d, int refl)
{
    tnear = MIN_TRAVEL; tfar = INFIN; intersect = NULL;
    origin = o; direction = d; color.set(0,0,0);
    n_refl = refl;
}


Ray::Ray(const Ray& in, const Vector3& norm)
    :tnear(MIN_TRAVEL), tfar(INFIN), intersect(NULL),
    origin(in.intersect_pt), direction(in.direction),
    color(),  n_refl(in.n_refl+1)
{
    //r -- reflect dir vect,  n -- norm vect, v -- input dir vect
    //r <= v + 2( v dot n )n
    direction += -2*(direction*norm)*norm;
}

const Vector3& Ray::get_dir()
{
    return direction;
}

const Vector3& Ray::get_origin()
{
    return origin;
}


RGBscale Ray::get_color()
{
    return color;
}

void Ray::set_color(RGBscale c)
{
    color = c;
}

//Allow reflect?
bool Ray::canReflect() {
    return n_refl < 4;
    //Can add a threshold value too into the test
}

