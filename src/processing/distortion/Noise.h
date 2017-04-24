#ifndef SRCCMAKE_NOISE_H
#define SRCCMAKE_NOISE_H

#include "processing/ImageFilter.h"

class Noise : public ImageFilter
{
public:
    Noise()
    {
        m_name = "Noise";
        m_filterGroup = "distortion";

        m_noiseMin = -0.5f;
        m_noiseMax = 0.5f;
        m_minFixed = -1.f;
        m_maxFixed = 1.f;
        m_options = {"Normal", "Salt n Pepper"};
        m_selectedOption = 0;

        m_interactableCollection.addInteractable(
                new FSlider("Noise min", &m_noiseMin, &m_minFixed, &m_noiseMax)
        );
        m_interactableCollection.addInteractable(
                new FSlider("Noise max", &m_noiseMax, &m_noiseMin, &m_maxFixed)
        );
        m_interactableCollection.addInteractable(
                new SelectBox("Type", &m_selectedOption, &m_options)
        );
    };
    ~Noise() {}

    void process();

private:
    float m_noiseMin;
    float m_noiseMax;
    float m_minFixed;
    float m_maxFixed;
    std::vector<std::string> m_options;
    int m_selectedOption;
};


#endif //SRCCMAKE_NOISE_H
