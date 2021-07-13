#include "RGBscale.h"
#include <stdexcept>

RGBscale::RGBscale()
    :sr(0), sg(0), sb(0)
{}


RGBscale::RGBscale(float r, float g, float b)
    :sr(r), sg(g), sb(b)
{
    if(r>1 || r<0 || g > 1 || g < 0 || b > 1 || b < 0)
        throw std::invalid_argument("Range [0,1] needed for color");
}

RGBscale RGBscale::operator*(const RGBscale& rhs) const
{
    RGBscale tmp(*this);
    tmp.sr *= rhs.sr;
    tmp.sg *= rhs.sg;
    tmp.sb *= rhs.sb;
    return tmp;
}

RGBscale RGBscale::operator*(float s) const {
    RGBscale tmp(*this);
    tmp.sr *= s;
    tmp.sg *= s;
    tmp.sb *= s;
    return tmp;
}

RGBscale& RGBscale::operator+=(const RGBscale& rhs) {
    sr += rhs.sr;
    sg += rhs.sg;
    sb += rhs.sb;
    return *this;
}


// Get methods (will clip)
float RGBscale::r() const {
    return (sr>1)?1:(sr<0)?0:sr;
}

float RGBscale::g() const {
    return (sg>1)?1:(sg<0)?0:sg;
}

float RGBscale::b() const {
    return (sb>1)?1:(sb<0)?0:sb;
}

// Set methods
void RGBscale::set(float r, float g, float b) {
    if(r>1 || r<0 || g > 1 || g < 0 || b > 1 || b < 0)
        throw std::invalid_argument("Range [0,1] needed for color");
    
    sr = r; sg = g; sb = b;
}

void RGBscale::set_r(float x) {
    if(x>1 || x<0) throw std::invalid_argument("Range [0,1] needed for color");
    sr = x;
}

void RGBscale::set_g(float x) {
    if(x>1 || x<0) throw std::invalid_argument("Range [0,1] needed for color");
    sg = x;
}

void RGBscale::set_b(float x) {
    if(x>1 || x<0) throw std::invalid_argument("Range [0,1] needed for color");
    sb = x;
}

void RGBscale::clip()
{
    if(sr>1) sr = 1;
    if(sg>1) sg = 1;
    if(sb>1) sb = 1;
    
    if(sr<0) sr = 0;
    if(sg<0) sg = 0;
    if(sb<0) sb = 0;
}

bool RGBscale::nonzero()
{
    return sr>0 || sg>0 || sb>0;
}

std::ostream& operator<<(std::ostream& out, const RGBscale& rgb)
{
    out << "( " << rgb.r() << ", " << rgb.g() << ", " << rgb.b() << " )";
    return out;
}

