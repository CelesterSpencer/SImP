#ifndef SRCCMAKE_TESTFILTER_H
#define SRCCMAKE_TESTFILTER_H

#include "filters/ImageFilter.h"

class MixImages : public ImageFilter
{
public:
    MixImages();

    void process();

private:
    float m_alpha;
    float m_min;
    float m_max;
};


#endif //SRCCMAKE_TESTFILTER_H
