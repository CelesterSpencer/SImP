#ifndef SRCCMAKE_SIMPLIFYCOLORS_H
#define SRCCMAKE_SIMPLIFYCOLORS_H

#include "filters/ImageFilter.h"
#include "core/util/Types.h"

class SimplifyColors : public ImageFilter
{
public:
    SimplifyColors();

    void process();

private:
    void getClosestColor(float rIn, float gIn, float bIn, float& rOut, float& gOut, float& bOut);
    void hsvToRgb(float h, float s, float v, float& r, float& g, float& b);

    int m_resolution;
    int m_minResolution;
    int m_maxResolution;
};


#endif //SRCCMAKE_SIMPLIFYCOLORS_H
