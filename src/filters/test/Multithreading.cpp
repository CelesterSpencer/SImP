#include "Multithreading.h"

Multithreading::Multithreading()
{
    m_name = "Multithreading";
    m_filterGroup = "test";

    m_noiseMin = -0.5f;
    m_noiseMax = 0.5f;
    m_minFixed = -1.f;
    m_maxFixed = 1.f;

    m_interactableCollection.addInteractable(
            new FSlider("Noise min", &m_noiseMin, &m_minFixed, &m_noiseMax)
    );
    m_interactableCollection.addInteractable(
            new FSlider("Noise max", &m_noiseMax, &m_noiseMin, &m_maxFixed)
    );
};

void Multithreading::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    // iterate over image
    out->parallel([=](Image* img, int x, int y, int c) -> float
    {
        float random = rand() % int(m_noiseMax-m_noiseMin + 1) + m_noiseMin;
        float val = std::max(std::min(img->get(x, y, c) + random, 1.f), 0.f);
        return val;
    });

    returnImage(out);
}