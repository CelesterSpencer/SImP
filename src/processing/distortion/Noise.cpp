//
// Created by Windrian on 21.03.2017.
//

#include "Noise.h"

Image* Noise::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    switch(m_selectedOption) {
        case 0:
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    int random = rand() % m_noiseMax + 1 + m_noiseMin;

                    int r = std::max(std::min(in->get(x, y, Image::Channel::RED) + random, 255), 0);
                    int g = std::max(std::min(in->get(x, y, Image::Channel::GREEN) + random, 255), 0);
                    int b = std::max(std::min(in->get(x, y, Image::Channel::BLUE) + random, 255), 0);

                    out->set(r, x, y, Image::Channel::RED);
                    out->set(g, x, y, Image::Channel::GREEN);
                    out->set(b, x, y, Image::Channel::BLUE);
                }
            }
            break;
        case 1:
        std::cout << "Salt n pepper" << std::endl;
            for (int x = 0; x < in->getWidth(); x++) {
                for (int y = 0; y < in->getHeight(); y++) {
                    float random = (float) rand() / (RAND_MAX);

                    int r = (random > 0.9) ? 0 : (random > 0.8) ? 255 : in->get(x, y, Image::Channel::RED);
                    int g = (random > 0.9) ? 0 : (random > 0.8) ? 255 : in->get(x, y, Image::Channel::GREEN);
                    int b = (random > 0.9) ? 0 : (random > 0.8) ? 255 : in->get(x, y, Image::Channel::BLUE);

                    out->set(r, x, y, Image::Channel::RED);
                    out->set(g, x, y, Image::Channel::GREEN);
                    out->set(b, x, y, Image::Channel::BLUE);
                }
            }
            break;
    }

    return out;
}