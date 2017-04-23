//
// Created by Windrian on 21.03.2017.
//

#include "Noise.h"

void Noise::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    float minBound = 0.f;
    float maxBound = 255.0f;

    switch(m_selectedOption) {
        case 0: // normal noise
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    int random = rand() % m_noiseMax + 1 + m_noiseMin;

                    float r = std::max(std::min(in->get(x, y, Image::Channel::RED) + random, maxBound), minBound);
                    float g = std::max(std::min(in->get(x, y, Image::Channel::GREEN) + random, maxBound), minBound);
                    float b = std::max(std::min(in->get(x, y, Image::Channel::BLUE) + random, maxBound), minBound);

                    out->set(r, x, y, Image::Channel::RED);
                    out->set(g, x, y, Image::Channel::GREEN);
                    out->set(b, x, y, Image::Channel::BLUE);
                }
            }
            break;
        case 1: // salt and pepper noise
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    float random = (float) rand() / (RAND_MAX);

                    float r = (random > 0.9) ? 0.f : (random > 0.8) ? 255.f : in->get(x, y, Image::Channel::RED);
                    float g = (random > 0.9) ? 0.f : (random > 0.8) ? 255.f : in->get(x, y, Image::Channel::GREEN);
                    float b = (random > 0.9) ? 0.f : (random > 0.8) ? 255.f : in->get(x, y, Image::Channel::BLUE);

                    out->set(r, x, y, Image::Channel::RED);
                    out->set(g, x, y, Image::Channel::GREEN);
                    out->set(b, x, y, Image::Channel::BLUE);
                }
            }
            break;
    }

    returnImage(out);
}