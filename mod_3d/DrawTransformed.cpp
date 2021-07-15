#include "DrawTransformed.h"
#include "../mod_matrix/Matrix4.h"
#include "../mod_matrix/Vector3.h"
#include "../mod_matrix/Vector4.h"
#include <math.h>
#include "lines.h"
#include <stdlib.h>
using namespace std;

DrawTransformed::DrawTransformed(CommandHandle& ch): ModuleCommand(),
    handler(ch), use_perspect(false), orth(0,1), vertex_saved(false), dbg_mode(0)
{
    //Scene handling
    handler.add_command("push",*this);
    handler.add_command("pop",*this);

    //Normal transforms
    handler.add_command("translate",*this);
    handler.add_command("scale",*this);
    handler.add_command("rotate",*this);
    handler.add_command("ortho",*this);

    handler.add_command("orient",*this);

    //View transforms
    handler.add_command("perspective",*this);
    handler.add_command("lookat",*this);

    //Line draw
    handler.add_command("vertex",*this);

    //Clear drawing
    handler.add_command("reset",*this);

    //Debug commands
    handler.add_command("printc", *this);
    handler.add_command("printview", *this);
    handler.add_command("debug3d", *this);

    //Misc
    handler.add_command("resizewin", *this);
}


string DrawTransformed::execute(vector< string > argv)
{
    if(argv.size()==0)
        return "Must provide function";

    //Split off command
    string cmd = argv[0];
    argv.erase( argv.begin() );

    //Allow insertion of verbose flag (and a debug mode)
    if(dbg_mode!=2) dbg_mode = argv.size() && argv[0]=="-v";
    if(argv.size() && argv[0]=="-v") argv.erase( argv.begin() );

    if(cmd == "push") {
        check_args(0, argv);
        return push();
    }

    if(cmd == "pop") {
        check_args(0, argv);
        return pop();
    }

    if(cmd == "translate") {
        check_args(1,3,argv,"0");
        return translate( sanitize_float(argv[0]),
                          sanitize_float(argv[1]),
                          sanitize_float(argv[2]) );
    }

    if(cmd == "scale") {
        if(argv.size()==1) { //scale f => scale f f f
            argv.push_back(argv[0]);
            argv.push_back(argv[0]);
        }
        check_args(1,3,argv,"1");
        return scale( sanitize_float(argv[0]),
                      sanitize_float(argv[1]),
                      sanitize_float(argv[2]) );
    }

    if(cmd == "rotate") {
        check_args(1,4,argv,"0");
        return rotate( sanitize_float(argv[0]), sanitize_float(argv[1]),
                       sanitize_float(argv[2]), sanitize_float(argv[2]));
    }

    if(cmd == "orient") {
        check_args(9,argv);

        //Sanitize all floats
        float v[9];
        for(int i=0; i<9; i++)
            v[i] = sanitize_float(argv[i]);

        return orient(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8]);
    }

    if(cmd == "vertex") {
        check_args(0,4,argv,"0");
        return vertex(sanitize_float(argv[0]),
                      sanitize_float(argv[1]),
                      sanitize_float(argv[2]) );
    }

    if(cmd == "lookat") {
        check_args(9,argv);

        //Sanitize all floats
        float v[9];
        for(int i=0; i<9; i++)
            v[i] = sanitize_float(argv[i]);

        return lookat(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8]);
    }

    if(cmd == "ortho") {
        check_args(6,argv);
        return ortho(sanitize_float(argv[0]), sanitize_float(argv[1]),
                     sanitize_float(argv[2]), sanitize_float(argv[3]),
                     sanitize_float(argv[4]), sanitize_float(argv[5]) );
    }

    if(cmd == "perspective") {
        check_args(4, argv);

        return perspective(sanitize_float(argv[0]), sanitize_float(argv[1]),
                           sanitize_float(argv[2]), sanitize_float(argv[3]) );
    }

    if(cmd == "reset") {
        check_args(0,argv);
        return reset();
    }

    if(cmd == "printc") {
        cout << "Diagnostic: current Tform matrix:\n" << C.get() << endl;
        return "null";
    }

    if(cmd == "printview") {
        cout << "Using " << ((use_perspect)?"perspective:":"ortho:") << endl;
        if(use_perspect) cout << perspect << "Ortho Matrix:" << endl;
        cout << orth << endl;
        return "null";
    }

    if(cmd == "debug3d") {
        dbg_mode = ((dbg_mode==2)?0:2);
        cout << "Set debug mode to " << ((dbg_mode)?"on":"off") << endl;
        return "null";
    }

    if(cmd == "resizewin") {
        if(argv.size()==1) argv.push_back(argv[0]);
        check_args(0,2,argv,"250");
        float width = max(sanitize_float(argv[0]),(float)50);
        float height = max(sanitize_float(argv[1]),(float)50);
        glutReshapeWindow(width, height);
        glutPostRedisplay();
        return "null";
    }

    return "Unimplemented command '"+cmd+"'";
}

//Stack commands
string DrawTransformed::push()
{
    C.push();
    return "null";
}

string DrawTransformed::pop() {
    C.pop();
    return "null";
}

string DrawTransformed::reset() {
    //Make screen black
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glFlush();

    //Reset array to black
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = abs(viewport[2]-viewport[0]);
    int height = abs(viewport[3]-viewport[1]);
    for(int i=0; i<width && i<MAX_IMG_WIDTH; i++) {
        for(int j=0; j<height && j<MAX_IMG_HEIGHT; j++) {
            for(int k=0; k<3; k++) {
                handler.main_window[i][j][k] = (GLbyte) 0;
            }
        }
    }

    handler.execute(string("tiffreset")); //Reset img tags for write out


    //Reset stack
    C.reset();

    return "null";
}

//Transform commands
string DrawTransformed::translate(float x, float y, float z)
{
    Matrix4 Mt(1, 0, 0, x,
               0, 1, 0, y,
               0, 0, 1, z,
               0, 0, 0, 1);
    C.transform(Mt);
    return "null";
}

string DrawTransformed::scale(float sx, float sy, float sz)
{
    Matrix4 Ms(sx,  0,  0,  0,
                0, sy,  0,  0,
                0,  0, sz,  0,
                0,  0,  0,  1);
    C.transform(Ms);
    return "null";
}

string DrawTransformed::rotate(float phi, float ax, float ay, float az)
{
    Vector3 a(ax,ay,az), cdir;
    float sth, cth, d, an, ani, cal, sal, th;
    th = phi/180 * M_PI;
    sincosf(th, &sth, &cth);
    if(!a.norm()) a.k = 1; //Make sure we have a valid vector
    an = a.norm();
    ani = 1/an;
    cdir = ani*a;
    d = sqrt(cdir.j*cdir.j + cdir.k*cdir.k);

    sal = cdir.j/d;
    cal = cdir.k/d;

    Matrix4 Max(1,  0,    0,  0,
                0, cal, -sal, 0,
                0, sal,  cal, 0,
                0,  0,    0,  1 );

    Matrix4 May(  d,    0, -cdir.i, 0,
                  0,    1,    0,    0,
                cdir.i, 0,    d,    0,
                  0,    0,    0,    1 );

    Matrix4 Mrot(cth, -sth, 0, 0,
                 sth,  cth, 0, 0,
                 0,  0, 1, 0,
                 0,  0, 0, 1 );

    Matrix4 Mry, Mrx;
    Mry = Max.transpose();
    Mrx = May.transpose();


    Matrix4 Rotate;
    Rotate = Mrx * Mry * Mrot * May * Max;

    C.transform(Rotate);

    return "null";
}

string DrawTransformed::orient(float aa, float ab, float ac,
                               float ba, float bb, float bc,
                               float ca, float cb, float cc)
{
    Matrix4 Mt(aa, ab, ac, 0,
               ba, bb, bc, 0,
               ca, cb, cc, 0,
                0,  0,  0, 1 );
    C.transform(Mt);
    return "null";
}


//Viewing commands
string DrawTransformed::lookat(float fx, float fy, float fz,
                               float atx, float aty, float atz,
                               float upx, float upy, float upz)
{
    Matrix4 ltrans(0,1);
    ltrans.at(0,3) = -fx;
    ltrans.at(1,3) = -fy;
    ltrans.at(2,3) = -fz;

    Vector3 up(upx, upy, upz);
    up.make_unit();

    Vector3 rz(atx-fx, aty-fy, atz-fz);
    rz.make_unit();

    Vector3 rx = rz.cross(up);
    rx.make_unit();

    Vector3 ry = rx.cross(rz);

    Matrix4 rfin( rx.i, rx.j, rx.k, 0,
                  ry.i, ry.j, ry.k, 0,
                 -rz.i,-rz.j,-rz.k, 0,
                    0,     0,    0, 1);

    Matrix4 lookat;
    lookat = rfin*ltrans;

    C.transform(lookat);
    return "null";
}

//Viewing commands
string DrawTransformed::ortho(float l, float r, //x-axis (left, right)
                              float b, float t, //y-axis (bottom, top)
                              float n, float f) //z-axis (near, far)
{
    if(r<l) swap(l,r);
    if(t<b) swap(t,b);
    if(f<n) swap(n,f);

    //Get window size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = abs(viewport[2]-viewport[0]);
    int length = abs(viewport[3]-viewport[1]);

    //Perform calculations
    float xf = width/(r-l);
    float yf = length/(t-b);

    Matrix4 Mo(xf,  0, 0, -l*xf,
                0, yf, 0, -b*yf,
                0,  0, 1,   -n,
                0,  0, 0,    1  );

    clip_near = n;
    clip_far = f;
    //set_clip_window(-1,-1,1,1);

    orth = Mo;
    use_perspect = false;
    return "null";
}

string DrawTransformed::perspective(float phi, float a, float n, float f) {
    float c, s, tn, Q;
    sincosf(phi/360*M_PI, &s, &c);
    //float Q = s/(1-n/f);
    tn = s/c; // tangent

    Q = f/(f-n);

    Matrix4 Mp(n, 0, 0,   0,
               0, n, 0,   0,
               0, 0, Q, -Q*n,
               0, 0, 1,   0 );

    //Need to do screen ortho tform to view area
    Matrix4 Mo = orth; //Save
    if(dbg_mode)
        cout << "Screen ("<<-a*tn*n<<","<<-tn*n<<") to ("
            << a*tn*n << tn*n << ")" << endl;
    ortho(-a*tn*n, a*tn*n, -tn*n, tn*n, 0,1);
    //swap(Mo, orth);

    clip_near = -n;
    clip_far = -f;
    perspect = Mp;

    use_perspect = true;
    return "null";
}



//Line drawing commands
string DrawTransformed::vertex(float x, float y, float z)
{
    gtVertex3f(x,y,z);
    if(dbg_mode && !vertex_saved) return "pause"; //Slow things down...
    return "null";
}

void DrawTransformed::gtVertex3f(float x, float y, float z)
{
    Vector4 printvec(x,y,z,1);
    float x0, y0, z0, x1, y1, z1;
    //float xpixel1, ypixel1, xpixel2, ypixel2;
    Vector4 vertex1, vertex2, permat2;

    Vector4 tmp = C.get() * printvec;

    if(vertex_saved) {
        if(dbg_mode)
            cout << "Printing line from "<<vertex_save<<" to "<<tmp<<"." << endl;
        x1=tmp[0];
        y1=tmp[1];
        z1=tmp[2];

        x0=vertex_save[0];
        y0=vertex_save[1];
        z0=vertex_save[2];

        if(near_far_clip(clip_near, clip_far, &x0, &y0, &z0, &x1, &y1, &z1)==1) {
            Vector4 pvert1(x0,y0,z0,1);
            Vector4 pvert2(x1,y1,z1,1);
            if(dbg_mode)
                cout << "Clipped line: "<<pvert1<<" to "<<pvert2<<"."<<endl;

            if(use_perspect) {
                /* if a perspective projection, use persp */
                Vector4 permat1(x0/abs(z0),
                                y0/abs(z0),
                                1.0,
                                1.0/abs(z0));
                Vector4 permat2(x1/abs(z1),
                                y1/abs(z1),
                                1.0,
                                1.0/abs(z1));
                pvert1 = perspect * permat1;
                pvert2 = perspect * permat2;

              if(dbg_mode)
                cout << "Perspective tformed line: "<<pvert1<<" to "<<pvert2<<"."<<endl;
            }
            /* Apply ortho */
            vertex1 = orth*pvert1;
            vertex2 = orth*pvert2;

            if(dbg_mode)
                cout << "On screen: "<<vertex1<<" to "<<vertex2<<endl<<endl;
            draw_line(vertex1[0], vertex1[1],
                      vertex2[0], vertex2[1],
                      handler  );
        }
        vertex_saved = false;
    } else {
        vertex_save = tmp;
        vertex_saved = true;
    }
}


