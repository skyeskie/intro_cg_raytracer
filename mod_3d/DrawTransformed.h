#ifndef DRAWTRANSFORMED_H
#define DRAWTRANSFORMED_H

#include "../mod_base/ModuleCommand.h"
#include "../mod_base/CommandHandle.h"
#include "../mod_matrix/TransformStack.h"
#include <string>
#include <vector>

class DrawTransformed : public ModuleCommand
{
public:
    DrawTransformed(CommandHandle& ch);
    virtual std::string execute(std::vector< std::string > argv);

    std::string push();
    std::string pop();

    std::string translate(float x, float y, float z);

    std::string scale(float sx, float sy, float sz);

    std::string rotate(float th, float ax, float ay, float az);
    std::string lookat( float  fx, float  fy, float  fz,
                        float atx, float aty, float atz,
                        float upx, float upy, float upz);
    
    std::string ortho(float l, float r, float b, float t, float n, float f);
    
    std::string perspective(float phi, float a, float n, float f);

    std::string vertex(float x, float y, float z);

    std::string reset();

    std::string orient(float aa, float ab, float ac,
                       float ba, float bb, float bc,
                       float ca, float cb, float cc);
    
private:
    CommandHandle& handler;

    TransformStack C;

    //Provided code (adapted in)
    typedef struct
    {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
    } Vertex_unit;

    bool use_perspect;


    
    Matrix4 orth;
    Matrix4 perspect;

    void gtVertex3f(float x, float y, float z);

    float clip_near;
    float clip_far;
    
    Vector4 vertex_save;
    bool vertex_saved;

    int dbg_mode;
};

#endif // DRAWTRANSFORMED_H
