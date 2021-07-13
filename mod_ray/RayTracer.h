#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../mod_base/ModuleCommand.h"
#include "../mod_base/CommandHandle.h"

#include "../mod_geo/Shape.h"
#include "../mod_geo/RGBscale.h"

#include "../mod_matrix/Vector3.h"

#include "Light.h"
#include "Screen.h"
#include <vector>

class RayTracer : public ModuleCommand
{
public:
    RayTracer(CommandHandle& ch);
    ~RayTracer();
    
    virtual std::string execute(std::vector<std::string> argv);

    void execute(std::string cmd, std::vector<std::string>& argv);

    void clear();

    void trace();

    void debug_trace(int x, int y);

    void set_bgcolor(float r, float g, float b);

    void set_camera();

    void intersect(Ray& r, Shape* ignore = NULL);

    void light_calc(Ray& r);    

private:
    CommandHandle& handler;

    std::vector<Shape*> objects;
    std::vector<Light*> lights;

    Screen* viewscreen;
    int window_len;
    int window_wid;

    void init_screen();
    void put_screen(Ray& r, int x, int y);

    RGBscale bg_color;

    bool debug_mode;
    void indent(Ray& r);
};

#endif // RAYTRACER_H
