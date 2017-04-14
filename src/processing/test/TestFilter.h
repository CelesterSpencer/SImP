#ifndef SRCCMAKE_TESTFILTER_H
#define SRCCMAKE_TESTFILTER_H

#include "processing/ImageFilter.h"

class TestFilter : public ImageFilter
{
public:
    TestFilter() {
        m_name = "Test filter";
        m_filterGroup = "test";
        m_alpha = 0.5;
        m_min = 0.0;
        m_max = 1.0;

        m_interactableCollection.addInteractable(new ImageSelector("Image 1"));
        m_interactableCollection.addInteractable(new ImageSelector("Image 2"));
        m_interactableCollection.addInteractable(new FSlider("mix", &m_alpha, &m_min, &m_max));
    };
    ~TestFilter() {};

    void process();

private:
    float m_alpha;
    float m_min;
    float m_max;
};


#endif //SRCCMAKE_TESTFILTER_H
