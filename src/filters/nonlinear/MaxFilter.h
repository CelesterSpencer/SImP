#ifndef SRCCMAKE_MAXFILTER_H
#define SRCCMAKE_MAXFILTER_H

#include "filters/ImageFilter.h"

class MaxFilter : public ImageFilter
{
public:
    MaxFilter() {
        /*
         * mandatory filter parameters
         */
        m_name = "Max Filter";
        m_filterGroup = "non linear";

        /*
         * interactables the user can interact with
         */
        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius)
        );
    };
    ~MaxFilter() {};

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;
};


#endif //SRCCMAKE_MAXFILTER_H