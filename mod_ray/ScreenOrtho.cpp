#include "ScreenOrtho.h"

ScreenOrtho::ScreenOrtho(int x, int y): Screen(x, y)
{
}


Ray ScreenOrtho::get_ray(int x, int y) {
    return Ray(Vector3(x,y,0), Vector3(0,0,-1));
}
