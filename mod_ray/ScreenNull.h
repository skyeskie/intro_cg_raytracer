#ifndef SCREEN_NULL_H
#define SCREEN_NULL_H
#include "Ray.h"
#include "Screen.h"
#include <stdexcept>
using namespace std;

class ScreenNull : public Screen {
public:
    ScreenNull() : Screen(0,0) {};
    virtual Ray get_ray(int x, int y)  { err(); return Ray(); };

private:
    void err() {
        throw runtime_error("No screen set. Cannot get ray.");
    };
};

#endif