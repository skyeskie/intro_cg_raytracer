#ifndef IMAGERESIZE_H
#define IMAGERESIZE_H

#include <vector>
#include <string>
#include "../mod_base/ModuleCommand.h"
#include "../mod_base/CommandHandle.h"

class ImageResize : public ModuleCommand
{
public: //Main command module functions
    //ImageResize();

    ImageResize(CommandHandle& );

    virtual std::string execute(std::vector< std::string> argv);


    std::string zoom(float);

    std::string resize(float, float);



private: //Data structures
    struct Pixel {
        Pixel() {};
        Pixel(GLubyte gb[3])
            :r(gb[0]), g(gb[1]), b(gb[2])
        {};

        int r, g, b;
    };

    struct PixelFl {
        PixelFl() {};
        
        PixelFl(Pixel p)
            :r(p.r), g(p.g), b(p.b)
        {};
        
        float r, g, b;

        void reset() { r=0; g=0; b=0; };
        Pixel extract(float w = 1.0);

        PixelFl& operator+=(PixelFl& rhs);
    };
    
    PixelFl scale(Pixel, float sf);

    Pixel* srcdata;
    Pixel* tmpdata;
    int len, wid;

    //Kernel function to use
    float (ImageResize::*filter)(float);

    std::string filter_name;

    int filter_width;

    int border_type;

    CommandHandle& handler;

//Helper methods
    //Copies screen from CH to srcdata
    void get_screen(bool flipx=0, bool flipy=0);

    void put_screen(Pixel*, size_t, size_t);


    //Resize no border
    void resample_main(Pixel* seq_start, Pixel* seq_target,
                       float xl, float xh, int xl_new, int n, int step);

    void resample_edge(Pixel* seq_start, Pixel* seq_target, int len,
                       float xl, float xh, int xl_new, int n, int step);
    
    std::string set_border(std::string );

    //Filter Functions
    std::string set_filter(std::string, size_t f_width = 0);

    float f_lanczos(float);

    float sinc(float);
    //Additional filters (extra credit)
    float f_gaussian(float );

    float f_mitchell(float );

    float f_raise_cos(float, float);
    float f_hamming(float );
    float f_hann(float );

    float f_tent(float ); //triangle

    float f_box(float );//*/
};

//typedef float (ImageResize::*FilterFun)(float);

#endif // IMAGERESIZE_H
