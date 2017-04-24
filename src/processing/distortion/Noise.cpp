#include "Noise.h"

void Noise::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    float minBound = 0.f;
    float maxBound = 1.f;

    switch(m_selectedOption) {
        case 0: // normal noise
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    float random = rand() % int(m_noiseMax-m_noiseMin + 1) + m_noiseMin;
                    for (int c = 0; c < in->getChannelNumber(); c++)
                    {
                        float val = std::max(std::min(in->get(x, y, c) + random, maxBound), minBound);
                        out->set(val, x, y, c);
                    }
                }
            }
            break;
        case 1: // salt and pepper noise
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    float random = (float) rand() / (RAND_MAX);
                    for (int c = 0; c < in->getChannelNumber(); c++)
                    {
                        float val = (random > 0.9) ? 0.f : (random > 0.8) ? 1.f : in->get(x, y, c);
                        out->set(val, x, y, c);
                    }
                }
            }
            break;
    }

    returnImage(out);
}