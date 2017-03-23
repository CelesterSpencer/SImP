//
// Created by Windrian on 22.03.2017.
//

#include "MaxFilter.h"

Image* MaxFilter::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float maxR = 0;
            float maxG = 0;
            float maxB = 0;
            // iterate over filter
            for(int fx = -m_filterRadius; fx <= m_filterRadius; fx++)
            {
                for (int fy = -m_filterRadius; fy <= m_filterRadius; fy++)
                {
                    int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                    int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                    maxR = std::max(in->get(posX, posY, Image::Channel::RED),   (int) maxR);
                    maxG = std::max(in->get(posX, posY, Image::Channel::GREEN), (int) maxG);
                    maxB = std::max(in->get(posX, posY, Image::Channel::BLUE),  (int) maxB);
                }
            }
            out->set((int) maxR, x, y, Image::Channel::RED);
            out->set((int) maxG, x, y, Image::Channel::GREEN);
            out->set((int) maxB, x, y, Image::Channel::BLUE);

        }
    }

    return out;
}