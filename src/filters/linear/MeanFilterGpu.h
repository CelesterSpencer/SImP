#ifndef SRCCMAKE_MEANFILTERGPU_H
#define SRCCMAKE_MEANFILTERGPU_H

#include "core/scene/ImageFilterGpu.h"

class MeanFilterGpu : public ImageFilterGpu
{
public:
    MeanFilterGpu()
    {
        // init gpu filter
        initGpu("Mean Filter", "linear", "/linear/MeanFilter");

        // output images must be specified for gpu filter
        addOutputImageDescription("Mean filtered image", 0, 0, 0);

        // define user input
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;

        addUserInputGpu("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius, "filterRadius");
    }

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MEANFILTERGPU_H
