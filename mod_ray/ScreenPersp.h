#ifndef SCREENPERSP_H
#define SCREENPERSP_H

#include "Screen.h"


class ScreenPersp : public Screen
{
public:
    ScreenPersp(int resx, int resy, const Vector3& eye, const Vector3& gaze,
                const Vector3& up, float dist, float au, float av, float bu, float bv );
    
    virtual Ray get_ray(int x, int y);
    
private:    
    Vector3 camera_eye;
    Vector3 camera_gaze;
    Vector3 camera_up;
    Vector3 camera_right;
    
    Vector3 screen_a;//3rd param is dist to screen
    Vector3 screen_b;//3rd param is dist to screen

    Vector3 del;
};

#endif // SCREENPERSP_H
