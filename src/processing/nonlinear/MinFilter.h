#ifndef SRCCMAKE_MINFILTER_H
#define SRCCMAKE_MINFILTER_H

#include "processing/ImageFilter.h"

class MinFilter : public ImageFilter
{
public:
    MinFilter()
    {
        m_name = "Min Filter";
        m_filterGroup = "non linear";

        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius)
        );
    };
    ~MinFilter() {};

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MINFILTER_H
