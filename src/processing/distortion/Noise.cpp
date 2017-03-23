//
// Created by Windrian on 21.03.2017.
//

#include "Noise.h"

Image* Noise::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            int random = rand() % m_noiseMax+1 + m_noiseMin;

            int r = std::max(std::min(in->get(x,y,Image::Channel::RED)+random,255),0);
            int g = std::max(std::min(in->get(x,y,Image::Channel::GREEN)+random,255),0);
            int b = std::max(std::min(in->get(x,y,Image::Channel::BLUE)+random,255),0);

            out->set(r, x, y,Image::Channel::RED);
            out->set(g, x, y,Image::Channel::GREEN);
            out->set(b, x, y,Image::Channel::BLUE);
        }
    }

    return out;
}