#ifndef SRCCMAKE_NOISE_H
#define SRCCMAKE_NOISE_H

#include "filters/ImageFilter.h"

class Noise : public ImageFilter
{
public:
    Noise();

    void process();

private:
    float m_noiseMin;
    float m_noiseMax;
    float m_minFixed;
    float m_maxFixed;
    std::vector<std::string> m_options;
    int m_selectedOption;
};


#endif //SRCCMAKE_NOISE_H
