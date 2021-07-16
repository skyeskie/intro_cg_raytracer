#include "ScreenPersp.h"
#include "../mod_matrix/Matrix3.h"
#include <iostream>
using namespace std;

ScreenPersp::ScreenPersp(int resx, int resy, const Vector3& eye,
                         const Vector3& gaze, const Vector3& up,
                         float dist, float au, float av, float bu, float bv)
    : Screen(resx, resy),
    camera_eye(eye), camera_gaze(gaze), camera_up(up),
    screen_a(au, av, dist), screen_b(bu, bv, dist)
{
    camera_gaze.make_unit();
    camera_up.make_unit();

    //3rd coord
    camera_right = camera_gaze.cross(camera_up);
    camera_up = camera_right.cross(camera_gaze);

    //cout << "  Look: " << camera_gaze << endl;
    //cout << "  Up: " << camera_up << endl;
    //cout << "  Right: " << camera_right << endl;

    del = screen_b - screen_a;
    del.i /= resx;
    del.j /= resy;
    del.k = dist;
}

Ray ScreenPersp::get_ray(int x, int y)
{
    Vector3 c(x, y, 0);
    c.i *= del.i;
    c.j *= del.j;
    c += screen_a;

    Matrix3 M(camera_right ,camera_up, camera_gaze);

    Vector3 dir = M*c;

    return Ray(camera_eye, dir);
}
