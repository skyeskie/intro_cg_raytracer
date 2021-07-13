#ifndef SCREENORTHO_H
#define SCREENORTHO_H

#include "Screen.h"

class ScreenOrtho : public Screen
{
public:
    ScreenOrtho(int x, int y);
    
    virtual Ray get_ray(int x, int y);
};

#endif // SCREENORTHO_H
