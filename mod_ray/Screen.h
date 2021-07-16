#ifndef SCREEN_H
#define SCREEN_H
#include "Ray.h"

class Screen {
public:
    Screen(int x, int y);
    
    virtual Ray get_ray(int x, int y) = 0;

    int get_wid();
    int get_len();

private:
    int window_wid;
    int window_len;
};

#endif