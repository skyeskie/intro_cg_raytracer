#include "ImageResize.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <map>
using namespace std;

const float PI = 3.14159265;

/*ImageResize::ImageResize() : ModuleCommand()
{
    set_filter("lanczos"); //Default filter
}//*/

enum BORDER_TYPES { B_ZERO, B_FREEZE, B_CIRCULAR, B_TRUNC };
enum FILTER_TYPES { F_LANCZ, F_GAUSS, F_MITCH, F_HAMNG, F_HANN, F_TENT, F_BOX };

ImageResize::ImageResize(CommandHandle& ch): ModuleCommand(),
    filter_width(3), border_type(B_ZERO), handler(ch)
{
    //Register commands
    ch.add_command("resize", *this);
    ch.add_command("scale", *this);
    ch.add_command("border", *this);
    ch.add_command("select", *this);

    //Set default filter
    set_filter("lanczos");
}

string ImageResize::execute(vector< string > argv)
{
    if(argv.size()==0)
        return "Must provide function";
    
    //Split off command
    string cmd = argv[0];
    argv.erase( argv.begin() );

    //cout << "Input: "<<argv[0]<<" => "<<sanitize_float(argv[0])<<endl;
    switch( argv.size() ) {
        case 0:
            if(cmd=="border")
                return set_border("");
            
            if(cmd=="select")
                return set_filter("", 0);

            break;
        
        case 1:
            if(cmd=="scale" || cmd=="resize")
                return zoom( sanitize_float(argv[0]) );

            if(cmd=="border")
                return set_border(argv[0]);

            if(cmd=="select")
                return set_filter(argv[0], 0);
            
            break;

        case 2:
            if(cmd=="resize")
                return resize(sanitize_float(argv[0]), sanitize_float(argv[1]));

            if(cmd=="select")
                return set_filter(argv[0], (int) abs(sanitize_float(argv[1])));
            
        default:
            break;
    }
    
    //Only here if failed to match
    if(cmd=="resize" || cmd=="zoom" || cmd=="border" || cmd=="select")
        throw runtime_error("Invalid number of arguments for function "+cmd);
    else
        throw runtime_error("Unknown command "+cmd+" passed to ImageResize module.");
}

string ImageResize::set_border(string type) {
    //Case insensitive check
    handler.str_to_lower(type);

    if(type=="zero") {
        border_type = B_ZERO;
    } else if(type=="freeze") {
        border_type = B_FREEZE;
    } else if(type=="circular") {
        border_type = B_CIRCULAR;
    } else if(type=="truncate") {
        border_type = B_TRUNC;
    } else {
        if(type!="") cout << "Invalid type.\n";
        cout << "Current border method: ";
        switch(border_type) {
            case B_FREEZE:
                cout << "Freeze Border";
                break;
            case B_CIRCULAR:
                cout << "Wrap Image";
                break;
            case B_TRUNC:
                cout << "Truncate Filter";
                break;
            case B_ZERO:
            default:
                border_type = B_ZERO;
                cout << "Zero";
                break;
        }
        cout << endl;
    }
    return "null";
}


string ImageResize::set_filter(string f_name, size_t f_width) {
    //case insensitive
    handler.str_to_lower(f_name);

    map<string, int> filters;
    filters["lanczos"] = F_LANCZ; 
    filters["gaussian"] = F_GAUSS; 
    filters["mitchell"] = F_MITCH; 
    filters["hamming"] = F_HAMNG; 
    filters["hann"] = F_HANN; 
    filters["tent"] = F_TENT;
    filters["triangle"] = F_TENT;
    filters["box"] = F_BOX; 
    
    if(filters.find(f_name)==filters.end()) {
        if(f_name!="") cout << "Unknown filter '"<<f_name<<"'"<<endl;
        cout << "Current filter kernel: " << filter_name
             << " width " << filter_width << endl;
    }

    filter_name = f_name;
    switch( filters[f_name] ) {
        case F_GAUSS:
            filter = &ImageResize::f_gaussian;
            filter_width = (f_width)?f_width:2;
            break;

        case F_MITCH:
            filter = &ImageResize::f_mitchell;
            filter_width = (f_width)?f_width:2;
            break;

        case F_HAMNG:
            filter = &ImageResize::f_hamming;
            filter_width = (f_width)?f_width:3;
            break;

        case F_HANN:
            filter = &ImageResize::f_hann;
            filter_width = (f_width)?f_width:3;
            break;

        case F_TENT:
            filter = &ImageResize::f_tent;
            filter_width = (f_width)?f_width:2;
            break;

        case F_BOX:
            filter = &ImageResize::f_box;
            filter_width = (f_width)?f_width:2;
            break;
            
        case F_LANCZ:
        default:
            filter = &ImageResize::f_lanczos;
            filter_name = "lanczos";
            filter_width = (f_width)?f_width:2;
            break;            
    }
    
    return "null";
}

//Screen functions
void ImageResize::get_screen(bool flipx, bool flipy) {
    len = handler.window_height;
    wid = handler.window_width;

    srcdata = new Pixel[len*wid];


    if(flipx && flipy) {
        for(int i=0; i<len; i++) {
            for(int j=0; j<wid; j++) {
                (srcdata + i*wid + j)->r = handler.main_window[len-1-i][wid-1-j][0];
                (srcdata + i*wid + j)->g = handler.main_window[len-1-i][wid-1-j][1];
                (srcdata + i*wid + j)->b = handler.main_window[len-1-i][wid-1-j][2];
            }
        }
    } else if(flipx) {
        for(int i=0; i<len; i++) {
            for(int j=0; j<wid; j++) {
                (srcdata + i*wid + j)->r = handler.main_window[i][wid-1-j][0];
                (srcdata + i*wid + j)->g = handler.main_window[i][wid-1-j][1];
                (srcdata + i*wid + j)->b = handler.main_window[i][wid-1-j][2];
            }
        }
    } else if(flipy) {
        for(int i=0; i<len; i++) {
            for(int j=0; j<wid; j++) {
                (srcdata + i*wid + j)->r = handler.main_window[len-1-i][j][0];
                (srcdata + i*wid + j)->g = handler.main_window[len-1-i][j][1];
                (srcdata + i*wid + j)->b = handler.main_window[len-1-i][j][2];
            }
        }
    } else {
        for(int i=0; i<len; i++) {
            for(int j=0; j<wid; j++) {
                (srcdata + i*wid + j)->r = handler.main_window[i][j][0];
                (srcdata + i*wid + j)->g = handler.main_window[i][j][1];
                (srcdata + i*wid + j)->b = handler.main_window[i][j][2];
            }
        }
    }
}

void ImageResize::put_screen(ImageResize::Pixel* data, size_t wid, size_t len) {
    if(wid>1024 || len>1024) throw invalid_argument("Too big for screen size");

    handler.window_height = len;
    handler.window_width = wid;
    
    for(int i=0; i<(int) len; i++) {
        for(int j=0; j<(int) wid; j++) {
            handler.main_window[i][j][0] = (data + i*wid + j)->r;
            handler.main_window[i][j][1] = (data + i*wid + j)->g;
            handler.main_window[i][j][2] = (data + i*wid + j)->b;
        }
    }

    cout << "Image resized to " << wid << "x" << len << "pixels.\n" << endl;
    
    glutReshapeWindow(wid, len);
    glutPostRedisplay();
}


//Main resampling functions
string ImageResize::zoom(float zf) {
    cout << "Zoom factor: "<<zf<<endl;
    return resize(zf, zf);
}

string ImageResize::resize(float xf, float yf)
{
    if(!xf || !yf)
        throw invalid_argument("Cannot have scale factor of 0.");

    //Handle flip on negative
    get_screen((xf<0), (yf<0));
    xf = abs(xf);
    yf = abs(yf);
    
    int new_wid = xf*(float)wid;
    int new_len = yf*(float)len;

    cout << "Resizing ("<<wid<<"x"<<len<<") x["<<xf<<", "<<yf<<"] => "
         << "(" << new_wid<< "x" << new_len << ")" << endl;
    if(new_wid>1024 || new_len>1024)
        throw invalid_argument("Exceeds max size screen size");

    int r = filter_width;
    int new_rx = r*xf;
    int new_ry = r*yf;
    //tmpdata = srcdata;
    //srcdata = NULL;
    
    tmpdata = new Pixel[ new_wid*len ];

    if(wid>2*r) {
        //Resize in x
        for(int y=0; y<len; y++) {
            resample_main(srcdata+y*wid, tmpdata+y*new_wid,
                        r, wid-r, new_rx, new_wid-2*new_rx, 1);

            resample_edge(srcdata+y*wid, tmpdata+y*new_wid, wid, 0, r, 0, new_rx, 1);
            resample_edge(srcdata+y*wid, tmpdata+y*new_wid, wid,
                        wid-r, wid, new_wid-new_rx, new_rx, 1);
        }
    } else {
        //Do border checking on all pixels
        for(int y=0; y<len; y++) {
            resample_edge(srcdata+y*wid, tmpdata+y*new_wid, wid,
                        0, wid, 0, new_wid, 1);
        }
    }

    delete[] srcdata;
    srcdata = new Pixel[ new_wid*new_len ];

    if(len>2*r) {
        //Resize in y
        for(int x=0; x<new_wid; x++) {
            resample_main(tmpdata+x, srcdata+x,
                        r, len-r, new_ry, new_len-2*new_ry, new_wid);

            resample_edge(tmpdata+x, srcdata+x, len, 0, r, 0, new_ry, new_wid);
            resample_edge(tmpdata+x, srcdata+x, len,
                        len-r, len, new_len-new_ry, new_ry, new_wid);
        }
    } else {
        //Do border checking on all pixels
        for(int x=0; x<new_wid; x++) {
            resample_edge(tmpdata+x, srcdata+x, len, 0, len, 0, new_len, new_wid);
        }
    }

    //Write results
    put_screen(srcdata, new_wid, new_len);

    delete[] tmpdata;
    tmpdata = NULL;
    delete[] srcdata;
    srcdata = NULL;

    return "null";
}


void ImageResize::resample_main(ImageResize::Pixel* seq_start,
                                ImageResize::Pixel* seq_target,
                                float xl, float xh, int xl_new, int n, int step)
{
    float r = filter_width;
    float del = (xh - xl)/n;
    float x0 = xl + (del/2);
    float x, v, filter_weight;
    int y, off;
    off = xl/del*step;
    PixelFl s, cur;
    for(int i=0; i<n; i++) {
        s.reset(); //Reset to (0,0,0)
        x = x0 + i*del;

        filter_weight = 0;

        for(int j= ceil(x-r); j < floor(x+r); j++) {
            v = (*this.*filter)(x - j);
            filter_weight+=v;
            y = j*step;
            cur = scale(seq_start[y], v);
            s += cur;
        }
        
        seq_target[(xl_new+i)*step] = s.extract(filter_weight);
    }
}

void ImageResize::resample_edge(ImageResize::Pixel* seq_start,
                                ImageResize::Pixel* seq_target, int len,
                                float xl, float xh, int xl_new, int n, int step)
{
    /*
    Pixel z;
    z.r = 0;
    z.g = 255;
    z.b = 0;
    //float sf = n/(xh - xl)*step;
    for(int i=0; i<n; i++) {
        seq_target[(xl_new+i)*step] = z;
    }/*/
    float r = filter_width;
    float del = (xh - xl)/n;
    float x0 = xl + (del/2);
    float x, v, filter_weight;
    int y;
    PixelFl s, cur;
    for(int i=0; i<n; i++) {
        s.reset(); //Reset to (0,0,0)
        x = x0 + i*del;
        
        filter_weight = 0;
        
        for(int j= ceil(x-r); j < floor(x+r); j++) {
            v = (*this.*filter)(x - j);
            filter_weight+=v;
            //Check in range
            if(j<0 || j>= len) {
                //Handle according to appropriate method
                switch(border_type) {
                    case B_FREEZE:
                        //Get border pixel
                        if(j<0) y=0;
                        else y=(len-1)*step;
                        //and use that
                        cur = scale(seq_start[y], v);
                        s += cur;
                        break;
                    case B_CIRCULAR:
                        //Wrap around correct border
                        if(j<0) y=-j*step;
                        else y=(2*len-j-2)*step;
                        //and use that
                        cur = scale(seq_start[y], v);
                        s += cur;
                        break;
                    case B_TRUNC:
                        //Remove this pixel's weight
                        filter_weight-=v;
                        break;
                    case B_ZERO:
                    default:
                        //Do nothing
                        break;
                }
            } else {
                y = j*step;
                cur = scale(seq_start[y], v);
                s += cur;
            }
        }
        
        seq_target[(xl_new+i)*step] = s.extract(filter_weight);
    }//*/
}



float ImageResize::sinc(float x) {
    if(x==0) return 1;

    return sin(x)/x;
}


//Resize filters
float ImageResize::f_lanczos(float x) {
    float f = x;
    f = sinc(f)*sinc(f/filter_width);
    //f = min(f,255);
    //f = max(0,f);
    return f;
}

float ImageResize::f_tent(float x) {
    float f = abs(x);
    if(f>filter_width/2) return 0;
    return filter_width/2 - x;
}

float ImageResize::f_gaussian(float x)
{
    return (1/sqrt(2*PI*filter_width))*exp(-x*x/(2*filter_width^2));
}

float ImageResize::f_box(float x)
{
    return (filter_width/x-abs(x)>0)?0:1; //Normalization taken care of later
}

float ImageResize::f_hamming(float x) {
    return f_raise_cos(x, 0.54);
}

float ImageResize::f_hann(float x) {
    return f_raise_cos(x, 0.5);
}

inline
float ImageResize::f_raise_cos(float x, float a) {
    float f = abs(x);
    if(f>= filter_width/2) return 0;
    return a + (1-a)*cos( 2*PI*f / filter_width );
}

float ImageResize::f_mitchell(float x) {
    float f = abs(x);
    if(f>filter_width) return 0;
    if(f>filter_width/2) return (-7*pow(f,3) + 36*pow(f,2) - 60*f + 32)/18;
    return (21*pow(f,3) - 36*pow(f,2) + 16)/18;
}



//Dependent Structs
ImageResize::PixelFl ImageResize::scale(ImageResize::Pixel p, float sf)
{
    PixelFl tmp(p);
    tmp.r *= sf;
    tmp.g *= sf;
    tmp.b *= sf;
    return tmp;
}

ImageResize::PixelFl&
ImageResize::PixelFl::operator+=(ImageResize::PixelFl& rhs)  {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

ImageResize::Pixel ImageResize::PixelFl::extract(float w) {
    if(w==0) {
        r = g = b = 0;
    } else if(w != 1.0) {
        r /= w;
        g /= w;
        b /= w;
    }
    Pixel p;
    p.r = (r>0)?((r<255)?int(r):255):0;
    p.g = (g>0)?((g<255)?int(g):255):0;
    p.b = (b>0)?((b<255)?int(b):255):0;
    return p;
}

