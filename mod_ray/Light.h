#ifndef LIGHT_H
#define LIGHT_H

#include "../mod_geo/RGBscale.h"
#include "../mod_matrix/Vector3.h"

class Light
{
public:
    virtual RGBscale get_color() { return RGBscale(); };

    virtual Vector3 get_dir() { return Vector3(); };
};

#endif // LIGHT_H
