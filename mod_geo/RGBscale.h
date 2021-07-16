#ifndef RGBSCALE_H
#define RGBSCALE_H
#include <iostream>
class RGBscale
{
public:
    RGBscale();
    RGBscale(float r, float g, float b);

    void set(float r, float g, float b);

    RGBscale operator*(const RGBscale& rhs) const;
    RGBscale operator*(float s) const;
    RGBscale& operator+=(const RGBscale& rhs);

    // Get methods
    float r() const;
    float g() const;
    float b() const;

    // Mutate methods
    void set_r(float x);
    void set_g(float x);
    void set_b(float x);

    void clip();

    bool nonzero();

private:
    float sr;
    float sg;
    float sb;
};

std::ostream& operator<<(std::ostream& , const RGBscale& );

#endif // RGBSCALE_H
