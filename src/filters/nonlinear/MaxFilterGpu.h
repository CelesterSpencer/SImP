#ifndef SRCCMAKE_MAXFILTERGPU_H
#define SRCCMAKE_MAXFILTERGPU_H

#include "core/scene/ImageFilterGpu.h"

class MaxFilterGpu  : public ImageFilterGpu
{
public:
    MaxFilterGpu() {
        // init gpu filter
        initGpu("Max Filter", "non linear", "/nonlinear/MaxFilter");

        // output images must be specified for gpu filter
        addOutputImageDescription("Max filtered image", 0, 0, 0);

        // interactables the user can interact with
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;

        // filter radius can be modified
        addUserInputGpu("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius, "filterRadius");
    };

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};

#endif //SRCCMAKE_MAXFILTERGPU_H
