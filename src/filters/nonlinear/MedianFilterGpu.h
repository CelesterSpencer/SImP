#ifndef SRCCMAKE_MEDIANFILTERGPU_H
#define SRCCMAKE_MEDIANFILTERGPU_H

#include "filters/ImageFilterGpu.h"

class MedianFilterGpu  : public ImageFilterGpu
{
public:
    MedianFilterGpu() {
        /*
         * mandatory filter parameters
         */
        m_name = "Median Filter Gpu";
        m_filterGroup = "non linear";

        /*
         * gpu specific settings
         * its required to get gpu filters working
         */
        setComputeShader("/nonlinear/MedianFilter");
        addOutputImageDescription("Median filtered image", 0, 0, 0);

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
    ~MedianFilterGpu() {};

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};

#endif //SRCCMAKE_MEDIANFILTERGPU_H
