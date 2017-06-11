#ifndef SRCCMAKE_MEDIANFILTER_H
#define SRCCMAKE_MEDIANFILTER_H

#include <algorithm>

#include "filters/ImageFilter.h"

class MedianFilter : public ImageFilter
{
public:
    MedianFilter() {
        m_name = "Median Filter";
        m_filterGroup = "non linear";

        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius)
        );
    };
    ~MedianFilter() {};

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MEDIANFILTER_H
