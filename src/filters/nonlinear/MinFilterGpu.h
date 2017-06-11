#ifndef SRCCMAKE_MINFILTERGPU_H
#define SRCCMAKE_MINFILTERGPU_H

#include "filters/ImageFilterGpu.h"

class MinFilterGpu  : public ImageFilterGpu
{
public:
    MinFilterGpu() {
        /*
         * mandatory filter parameters
         */
        m_name = "Min Filter (Gpu)";
        m_filterGroup = "non linear";

        /*
         * gpu specific settings
         * its required to get gpu filters working
         */
        setComputeShader("/nonlinear/MinFilter");
        addOutputImageDescription("Min filtered image", 0, 0, 0);

        /*
         * interactables the user can interact with
         */
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius, "filterRadius")
        );
    };
    ~MinFilterGpu() {};

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};

#endif //SRCCMAKE_MINFILTERGPU_H
