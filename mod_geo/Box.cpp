#include "Box.h"
#include <limits>
using namespace std;

const float P_INF = numeric_limits<float>::infinity();
const float N_INF = -P_INF;

Box::Box(V3 x1, V3 x2, RGB amb, RGB refl, RGB spec)
    :Shape(amb, refl, spec), llcorner(x1), urcorner(x2)
{
    if(llcorner.i>urcorner.i) swap(llcorner.i, urcorner.i);
    if(llcorner.j>urcorner.j) swap(llcorner.j, urcorner.j);
    if(llcorner.k>urcorner.k) swap(llcorner.k, urcorner.k);
}

bool Box::intersect(Ray& r)
{
    Vector3 e = r.get_origin(),
            d = r.get_dir(),
            t0 = llcorner - e,
            t1 = urcorner - e;

    //Swap if needed
    if(d.i<0) swap(t0.i, t1.i);
    if(d.j<0) swap(t0.j, t1.j);
    if(d.k<0) swap(t0.k, t1.k);
            
    t0.i /= d.i;
    t0.j /= d.j;
    t0.k /= d.k;

    t1.i /= d.i;
    t1.j /= d.j;
    t1.k /= d.k;
    
    float tmax = P_INF,
          tmin = N_INF;

    //Find largest min
    if(t0.i > tmin) tmin = t0.i;
    if(t0.j > tmin) tmin = t0.j;
    if(t0.k > tmin) tmin = t0.k;

    //Find smallest max
    if(t1.i < tmax) tmax = t1.i;
    if(t1.j < tmax) tmax = t1.j;
    if(t1.k < tmax) tmax = t1.k;

    if(tmin > tmax) return false;
    if(r.tfar < tmin) return false;
    if(r.tnear > tmax) return false;

    //It hits
    r.intersect = this;
    r.tfar = tmin;
    r.intersect_pt = e+tmin*d;
    set_norm(r);
    return true;
}

Vector3 Box::get_norm(const Vector3& p)
{
    Vector3 del = urcorner - llcorner;
    Vector3 res;
    if(del.i) del.i /= del.i;
    if(del.j) del.j /= del.j;
    if(del.k) del.k /= -del.k;
    
    res.i = res.j = res.k = 0;
    
    float e0 = .005;
    Vector3 cl = p - llcorner;
    Vector3 cu = p - urcorner;
    
    if(cl.i<e0 && cl.i>-e0) res.i = del.i;
    if(cu.i<e0 && cu.i>-e0) res.i = -del.i;
    
    if(cl.j<e0 && cl.j>-e0) res.j = del.j;
    if(cu.j<e0 && cu.j>-e0) res.j = -del.j;
    
    if(cl.k<e0 && cl.k>-e0) res.k = del.k;
    if(cu.k<e0 && cu.k>-e0) res.k = -del.k;
    return res;
}

void Box::set_norm(Ray& r)
{
    Vector3 p = r.intersect_pt;
    
    Vector3 del = urcorner - llcorner;
    if(del.i) del.i /= del.i;
    if(del.j) del.j /= del.j;
    if(del.k) del.k /= del.k;

    r.norm.i = r.norm.j = r.norm.k = 0;

    float e0 = .005;
    Vector3 cl = p - llcorner;
    Vector3 cu = p - urcorner;

    if(cl.i<e0 && cl.i>-e0) r.norm.i = -del.i;
    if(cu.i<e0 && cu.i>-e0) r.norm.i = del.i;
    
    if(cl.j<e0 && cl.j>-e0) r.norm.j = -del.j;
    if(cu.j<e0 && cu.j>-e0) r.norm.j = del.j;

    if(cl.k<e0 && cl.k>-e0) r.norm.k = -del.k;
    if(cu.k<e0 && cu.k>-e0) r.norm.k = del.k;
}

Ray Box::reflect(const Ray& r)
{
    return Ray(r, r.norm);
}

