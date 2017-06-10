#ifndef SRCCMAKE_MAXFILTERGPU_H
#define SRCCMAKE_MAXFILTERGPU_H

#include "processing/ImageFilterGpu.h"

class MaxFilterGpu  : public ImageFilterGpu
{
public:
    MaxFilterGpu() {
        /*
         * mandatory filter parameters
         */
        m_name = "Max Filter (Gpu)";
        m_filterGroup = "non linear";

        /*
         * gpu specific settings
         * its required to get gpu filters working
         */
        setComputeShader("/nonlinear/MaxFilter");
        addOutputImageDescription("Max filtered image", 0, 0, 0);

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
    ~MaxFilterGpu() {};

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};

#endif //SRCCMAKE_MAXFILTERGPU_H
