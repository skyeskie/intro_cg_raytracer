#ifndef ILIGHT_H
#define ILIGHT_H

#include "Light.h"

class ILight : public Light
{
public:
    ILight(RGBscale, Vector3);

    RGBscale get_color();

    Vector3 get_dir();
    
private:
    RGBscale color;
    Vector3 direct;
};

#endif // LIGHT_H
