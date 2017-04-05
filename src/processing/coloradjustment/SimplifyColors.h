#ifndef SRCCMAKE_SIMPLIFYCOLORS_H
#define SRCCMAKE_SIMPLIFYCOLORS_H

#include "processing/ImageFilter.h"
#include "util/Types.h"

class SimplifyColors : public ImageFilter
{
public:
    SimplifyColors() {
        m_name = "Simplify colors";
        m_filterGroup = "color adjustment";

        m_resolution = 3;
        m_minResolution = 3;
        m_maxResolution = 30;

        m_interactableCollection.addInteractable(
            new ISlider("Number of colors", &m_resolution, &m_minResolution, &m_maxResolution)
        );
    };
    ~SimplifyColors() {};

    void process();

private:
    void getClosestColor(float rIn, float gIn, float bIn, float& rOut, float& gOut, float& bOut);
    void hsvToRgb(float h, float s, float v, float& r, float& g, float& b);

    int m_resolution;
    int m_minResolution;
    int m_maxResolution;
};


#endif //SRCCMAKE_SIMPLIFYCOLORS_H
