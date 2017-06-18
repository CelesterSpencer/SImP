#ifndef SRCCMAKE_MEDIANFILTERGPU_H
#define SRCCMAKE_MEDIANFILTERGPU_H

#include "core/scene/ImageFilterGpu.h"

class MedianFilterGpu  : public ImageFilterGpu
{
public:
    MedianFilterGpu() {
        // mandatory filter parameters
        initGpu("Median Filter", "non linear", "/nonlinear/MedianFilter");

        // output images must be specified for gpu filter
        addOutputImageDescription("Median filtered image", 0, 0, 0);

        // add user input
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;

        addUserInputGpu("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius, "filterRadius");
    };

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};

#endif //SRCCMAKE_MEDIANFILTERGPU_H
