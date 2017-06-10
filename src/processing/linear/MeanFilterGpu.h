#ifndef SRCCMAKE_MEANFILTERGPU_H
#define SRCCMAKE_MEANFILTERGPU_H

#include "processing/ImageFilterGpu.h"

class MeanFilterGpu : public ImageFilterGpu
{
public:
    MeanFilterGpu()
    {
        /*
         * mandatory filter parameters
         */
        m_name = "Mean Filter (Gpu)";
        m_filterGroup = "linear";

        /*
         * gpu specific settings
         * its required to get gpu filters working
         */
        setComputeShader("/linear/MeanFilter");
        addOutputImageDescription("Mean filtered image", 0, 0, 0);

        /*
         * interactables the user can interact with
         */
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius, "filterRadius")
        );
    }
    ~MeanFilterGpu() {};

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MEANFILTERGPU_H
