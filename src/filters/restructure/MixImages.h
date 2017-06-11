#ifndef SRCCMAKE_TESTFILTER_H
#define SRCCMAKE_TESTFILTER_H

#include "filters/ImageFilter.h"

class MixImages : public ImageFilter
{
public:
    MixImages() {
        m_name = "Mix Images";
        m_filterGroup = "restructure";
        m_alpha = 0.5;
        m_min = 0.0;
        m_max = 1.0;

        m_interactableCollection.addInteractable(new ImageSelector("Image 1"));
        m_interactableCollection.addInteractable(new ImageSelector("Image 2"));
        m_interactableCollection.addInteractable(new FSlider("mix", &m_alpha, &m_min, &m_max));
    };
    ~MixImages() {};

    void process();

private:
    float m_alpha;
    float m_min;
    float m_max;
};


#endif //SRCCMAKE_TESTFILTER_H
