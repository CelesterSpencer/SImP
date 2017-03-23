//
// Created by Windrian on 21.03.2017.
//

#include "MinFilter.h"

Image* MinFilter::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float minR = 255;
            float minG = 255;
            float minB = 255;
            // iterate over filter
            for(int fx = -m_filterRadius; fx <= m_filterRadius; fx++)
            {
                for (int fy = -m_filterRadius; fy <= m_filterRadius; fy++)
                {
                    int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                    int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                    minR = std::min(in->get(posX, posY, Image::Channel::RED),   (int)minR);
                    minG = std::min(in->get(posX, posY, Image::Channel::GREEN), (int)minG);
                    minB = std::min(in->get(posX, posY, Image::Channel::BLUE),  (int)minB);
                }
            }
            out->set((int)minR, x, y, Image::Channel::RED);
            out->set((int)minG, x, y, Image::Channel::GREEN);
            out->set((int)minB, x, y, Image::Channel::BLUE);

        }
    }

    return out;
}