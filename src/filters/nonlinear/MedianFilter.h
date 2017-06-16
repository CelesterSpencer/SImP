#ifndef SRCCMAKE_MEDIANFILTER_H
#define SRCCMAKE_MEDIANFILTER_H

#include <algorithm>

#include "filters/ImageFilter.h"

class MedianFilter : public ImageFilter
{
public:
    MedianFilter();

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MEDIANFILTER_H
