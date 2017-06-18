#ifndef SRCCMAKE_MINFILTERGPU_H
#define SRCCMAKE_MINFILTERGPU_H

#include "core/scene/ImageFilterGpu.h"

class MinFilterGpu  : public ImageFilterGpu
{
public:
    MinFilterGpu() {
        // mandatory filter parameters
        initGpu("Min Filter", "non linear", "/nonlinear/MinFilter");

        // output images must be specified for gpu filter
        addOutputImageDescription("Min filtered image", 0, 0, 0);

        // filter radius can be modified
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

#endif //SRCCMAKE_MINFILTERGPU_H
