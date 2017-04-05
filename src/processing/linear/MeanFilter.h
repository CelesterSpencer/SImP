#ifndef SRCCMAKE_MEANFILTER_H
#define SRCCMAKE_MEANFILTER_H

#include "processing/ImageFilter.h"

class MeanFilter: public ImageFilter
{
    friend class ImageFilter;
public:
    MeanFilter() {
        m_name = "Mean Filter";
        m_filterGroup = "linear";

        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius)
        );
    };
    ~MeanFilter() {};

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MEANFILTER_H