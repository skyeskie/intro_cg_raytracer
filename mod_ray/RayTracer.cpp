#include "RayTracer.h"
#include "ScreenNull.h"
#include "Ray.h"
#include <limits>
#include "ILight.h"
#include "../mod_geo/Sphere.h"
#include "ScreenOrtho.h"
#include "../mod_geo/Triangle.h"
#include "ScreenPersp.h"
#include "../mod_geo/Box.h"
using namespace std;

typedef RGBscale RGB;

const float INFINITY = numeric_limits<float>::infinity();

RayTracer::RayTracer(CommandHandle& ch): ModuleCommand(),
    handler(ch), viewscreen(new ScreenNull), window_len(250), window_wid(250),
    bg_color(0,0,0)
{
    ch.add_command("screen",*this);
    ch.add_command("orthocamera",*this);
    ch.add_command("camera",*this);
    ch.add_command("background",*this);
    ch.add_command("sphere",*this);
    ch.add_command("triangle",*this);
    ch.add_command("box",*this);
    ch.add_command("ilight",*this);
    ch.add_command("clear",*this);
    ch.add_command("trace",*this);
}

RayTracer::~RayTracer()
{
    for(vector<Shape*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        delete *it;
    }

    for(vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it) {
        delete *it;
    }
}



string RayTracer::execute(vector< string > argv) {
    if(argv.size()==0)
        return "Must provide function";
    //Split off command
        string cmd = argv[0];
        argv.erase( argv.begin() );

    execute(cmd, argv);
    
    return "null";
}

void RayTracer::execute(string cmd, vector<string>& argv) {
    if(cmd=="trace" && argv.size()==2)
        return debug_trace(sanitize_int(argv[0]), sanitize_int(argv[1]));
    
    //Parse no-arg commands first
    if(cmd=="clear") return clear();
    if(cmd=="trace") return trace();
    if(cmd=="orthocamera") {
        delete viewscreen;
        viewscreen = new ScreenOrtho(window_wid, window_len);
        cout << "Set orthocamera." << endl;
        return;
    }

    vector<float> argf;
    //Parse all as floats
    for(size_t i=0; i < argv.size(); i++) {
        argf.push_back(sanitize_float(argv[i]));
    }

    if(cmd=="screen") {
        check_args(2,argv);
        window_wid = sanitize_int(argv[0]);
        window_len = sanitize_int(argv[1]);
        cout << "Set screen resolution." << endl;
        return;
    }
    if(cmd=="camera") {
        check_args(14,argv);
        delete viewscreen;
        viewscreen = new ScreenPersp(window_wid, window_len,
                                     Vector3(argf[0], argf[1], argf[2]), //eye
                                     Vector3(argf[3], argf[4], argf[5]), //gaze
                                     Vector3(argf[6], argf[7], argf[8]), //up
                            //dist      au      av          bu        bv
                            argf[9], argf[10], argf[11], argf[12], argf[13] );
        cout << "Set camera." << endl;
        return;
    }
    if(cmd=="background") {
        check_args(0,3,argf,0.0);
        bg_color.set(argf[0], argf[1], argf[2]);
        cout << "Set background." << endl;
    }
    if(cmd=="sphere") {
        check_args(10,13,argf,0.0);
        Sphere* ns = new Sphere( Vector3(argf[1], argf[2], argf[3]), argf[0],
                RGB(argf[4], argf[5], argf[6]), //Ambient
                RGB(argf[7], argf[8], argf[9]), //Reflective
                RGB(argf[10], argf[11], argf[12]) ); //Specular
        objects.push_back(ns);
        cout << "Added sphere." << endl;
    }
    if(cmd=="triangle") {
        check_args(15,18,argf,0.0);
        Triangle* nt = new Triangle(Vector3(argf[0], argf[1], argf[2]), //v1
                                    Vector3(argf[3], argf[4], argf[5]), //v2
                                    Vector3(argf[6], argf[7], argf[8]), //v3
                                    RGB(argf[ 9], argf[10], argf[11]), //Ambient
                                    RGB(argf[12], argf[13], argf[14]), //Reflect
                                    RGB(argf[15], argf[16], argf[17]) //Specular
                           );
        objects.push_back(nt);
        cout << "Added triangle." << endl;
    }
    if(cmd=="box") {
        check_args(12, 15, argf, 0.0);
        Box* nb = new Box( Vector3(argf[0], argf[1], argf[2]), //v1
                           Vector3(argf[3], argf[4], argf[5]), //v2
                           RGB(argf[6], argf[7], argf[8]), //Ambient
                           RGB(argf[ 9], argf[10], argf[11]), //Reflect
                           RGB(argf[12], argf[13], argf[14]) //Specular
                      );
        objects.push_back(nb);
        cout << "Added box." << endl;
    }
    if(cmd=="ilight") {
        check_args(6, argv);
        ILight* tmp = new ILight( RGB(argf[0], argf[1], argf[2]),
                                Vector3(argf[3], argf[4], argf[5]) );
        lights.push_back(tmp);
        cout << "Added light at infinity." << endl;
        return;
    }
}



void RayTracer::clear() {
    objects.clear();
    lights.clear();
    delete viewscreen;
    viewscreen = new ScreenNull();
    bg_color.set(0,0,0);
}

void RayTracer::set_bgcolor(float r, float g, float b) {
    bg_color.set(r, g, b);    
}

void RayTracer::set_camera() {
    //Crazy...
}

void RayTracer::trace() {
    Ray r;
    debug_mode = false;
    init_screen();    
    for(int i=0; i<window_wid; i++) {
        for(int j=0; j<window_len; j++) {
            r = viewscreen->get_ray(i,j);
            intersect(r);
            light_calc(r);
            put_screen(r,i,j);
        }
    }
}

void RayTracer::debug_trace(int x, int y)
{
    debug_mode = true;
    Ray r = viewscreen->get_ray(x,y);
    cout << "Shooting ray " << r.get_origin() << " to " << r.get_dir() << endl;
    intersect(r);
    light_calc(r);
    put_screen(r,x,y);
}


void RayTracer::intersect(Ray& r, Shape* ignore) {
    //if(debug_mode) indent(r), cout << "Calculating intersection: ";
    vector<Shape*>::iterator it;
    for(it = objects.begin(); it != objects.end(); ++it) {
        //Allow an ignore object
        if(*it != ignore) (*it)->intersect(r);
    }
    //*/
    if(debug_mode) {
        indent(r);
        if(r.intersect == NULL) { cout << "Miss all." << endl; return; }
        cout << "Hit object " << r.intersect << endl;
        indent(r);
        cout << "  at " << r.intersect_pt << " and t = " << r.tfar << endl;
    }//*/
}

void RayTracer::light_calc(Ray& r) {
    /*~*/if(debug_mode) indent(r), cout << "Beginning Light Calc." << endl;
    Shape* obj = r.intersect;
    
    if(obj == NULL || r.tfar == INFINITY) { //Miss
        /*~*/if(debug_mode) indent(r), cout << "Miss. Setting to BG color: " << bg_color << endl;
        r.set_color(bg_color);
        return;
    }

    RGB color(0,0,0), spec, amb;
    spec = obj->get_specular_color();
    amb = obj->get_ambient_color();

    /*~*/if(debug_mode) indent(r), cout << "Can reflect? Ray: " << r.canReflect()
    /*~*/                              << " Surface: " << spec.nonzero() << endl;
    ///Reflection
    if(r.canReflect() && spec.nonzero()) {
        /*~*/if(debug_mode) indent(r), cout << "Can reflect. Reflection:" << endl;
        /*~*/if(debug_mode) indent(r), cout << "NORM: " << r.norm << endl;
        Ray refl;
        refl = obj->reflect(r);
        /*~*/if(debug_mode) indent(r), cout << refl.get_origin() << " to "
        /*~*/                             << refl.get_dir() << endl;
        intersect(refl, obj);
        light_calc(refl); //Recurse
        color += refl.get_color() * spec;
    }

    /*~*/if(debug_mode) indent(r), cout << "Calculating lights:" << endl;
    ///Run through lights
    Vector3 l;
    vector<Light*>::iterator it;
    Ray shadow;
    for(it = lights.begin(); it != lights.end(); ++it) {
        ///Normal diffuse calculation
        l = - (*it)->get_dir();
        ///Fire shadow ray
        /*~*/if(debug_mode) indent(r), cout << "  Firing shadow ray:\n", indent(r);
        shadow.reset(r.intersect_pt, l);
        /*~*/if(debug_mode) cout << "  " << shadow.get_origin() << " to "
        /*~*/                    << shadow.get_dir() << endl;
        intersect(shadow, obj); //Ignore self
        if(shadow.intersect == NULL) {
            ///Ok, light hits so just calc it
            /*~*/if(debug_mode) indent(r), cout << "  NORM: " << r.norm<<endl;
            l.make_unit();
            color += (obj->get_reflect_color() * (*it)->get_color())
                        * max(float(0), r.norm*l);
            /*~*/if(debug_mode) indent(r), cout << "   Color now "<<color<<endl;
        } /*~*/else if(debug_mode) indent(r), cout << "   Light obstructed." << endl;
    }
    color += amb;
    r.set_color(color);
    
    //color.clip();
    /*~*/if(debug_mode) indent(r), cout << "Adding in ambient light: " << amb << endl;
    /*~*/if(debug_mode) indent(r), cout << "Setting ray to color " << color << endl;
}

void RayTracer::init_screen() {
    handler.window_height = window_len;
    handler.window_width = window_wid;
    glutReshapeWindow(window_len, window_wid);
    glutPostRedisplay();
}


void RayTracer::put_screen(Ray& r, int x, int y) {
    RGB color = r.get_color();
    handler.main_window[y][x][0] = GLubyte( color.r() * 255 );
    handler.main_window[y][x][1] = GLubyte( color.g() * 255 );
    handler.main_window[y][x][2] = GLubyte( color.b() * 255 );
}

void RayTracer::indent(Ray& r)
{
    for(int i=0; i<r.depth(); i++) {
        cout << "\t";
    }
}


