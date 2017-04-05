//
// Created by Windrian on 21.03.2017.
//

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

        m_noiseMin = -125;
        m_noiseMax = 125;
        m_minFixed = -255;
        m_maxFixed = 255;
        m_options = {"Normal", "Salt n Pepper"};
        m_selectedOption = 0;

        m_interactableCollection.addInteractable(
                new ISlider("Noise min", &m_noiseMin, &m_minFixed, &m_noiseMax)
        );
        m_interactableCollection.addInteractable(
                new ISlider("Noise max", &m_noiseMax, &m_noiseMin, &m_maxFixed)
        );
        m_interactableCollection.addInteractable(
                new SelectBox("Type", &m_selectedOption, &m_options)
        );
    };
    ~Noise() {}

    void process();

private:
    int m_noiseMin;
    int m_noiseMax;
    int m_minFixed;
    int m_maxFixed;
    std::vector<std::string> m_options;
    int m_selectedOption;
};


#endif //SRCCMAKE_NOISE_H
