#ifndef SRCCMAKE_KUWAHARAFILTER_H
#define SRCCMAKE_KUWAHARAFILTER_H

#include "core/scene/ImageFilter.h"

class KuwaharaFilter : public ImageFilter
{
public:
    KuwaharaFilter();

    // inherited from ImageFilter
    void process();

private:
    int m_filterRadius;
    int m_radiusMin;
    int m_radiusMax;
};


#endif //SRCCMAKE_KUWAHARAFILTER_H
