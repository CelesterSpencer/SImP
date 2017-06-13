#ifndef SRCCMAKE_MINFILTER_H
#define SRCCMAKE_MINFILTER_H

#include "filters/ImageFilter.h"

class Multithreading : public ImageFilter
{
public:
    Multithreading()
    {
        m_name = "Multithreading";
        m_filterGroup = "test";

        m_filterRadius = 5;
        m_minRadius = 0;
        m_maxRadius = 10;
        m_interactableCollection.addInteractable(
                new ISlider("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius)
        );

        m_threadCount = 4;
        m_threadsMin = 1;
        m_threadsMax = 16;
        m_interactableCollection.addInteractable(
                new ISlider("Thread count", &m_threadCount, &m_threadsMin, &m_threadsMax)
        );
    };
    ~Multithreading() {};

    void process();

private:
    int m_filterRadius;
    int m_minRadius;
    int m_maxRadius;

    int m_threadCount;
    int m_threadsMin;
    int m_threadsMax;
};


#endif //SRCCMAKE_MINFILTER_H
