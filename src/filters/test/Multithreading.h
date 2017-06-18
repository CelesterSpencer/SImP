#ifndef SRCCMAKE_MINFILTER_H
#define SRCCMAKE_MINFILTER_H

#include "core/scene/ImageFilter.h"

class Multithreading : public ImageFilter
{
public:
    Multithreading();

    void process();

private:
    float m_noiseMin;
    float m_noiseMax;
    float m_minFixed;
    float m_maxFixed;
};


#endif //SRCCMAKE_MINFILTER_H
