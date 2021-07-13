#include "ILight.h"

ILight::ILight(RGBscale c, Vector3 d)
    :color(c), direct(d)
{
}


RGBscale ILight::get_color()
{
    return color;
}

Vector3 ILight::get_dir()
{
    return direct;
}
