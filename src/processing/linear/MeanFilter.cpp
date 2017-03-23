//
// Created by Windrian on 19.03.2017.
//

#include "MeanFilter.h"

Image* MeanFilter::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    int filterRadius = m_filterRadius;
    int filterSize = filterRadius*2 + 1;

    Image* temp = new Image;
    temp->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;
            // iterate over filter
            for(int fx = -filterRadius; fx <= filterRadius; fx++)
            {
                int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                sumR += in->get(posX, y, Image::Channel::RED);
                sumG += in->get(posX, y, Image::Channel::GREEN);
                sumB += in->get(posX, y, Image::Channel::BLUE);
            }
            sumR = sumR / filterSize;
            sumG = sumG / filterSize;
            sumB = sumB / filterSize;
            temp->set((int)sumR, x, y, Image::Channel::RED);
            temp->set((int)sumG, x, y, Image::Channel::GREEN);
            temp->set((int)sumB, x, y, Image::Channel::BLUE);

        }
    }

    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;
            // iterate over filter
            for (int fy = -filterRadius; fy <= filterRadius; fy++)
            {
                int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                sumR += temp->get(x, posY, Image::Channel::RED);
                sumG += temp->get(x, posY, Image::Channel::GREEN);
                sumB += temp->get(x, posY, Image::Channel::BLUE);
            }
            sumR = sumR / filterSize;
            sumG = sumG / filterSize;
            sumB = sumB / filterSize;
            out->set((int)sumR, x, y, Image::Channel::RED);
            out->set((int)sumG, x, y, Image::Channel::GREEN);
            out->set((int)sumB, x, y, Image::Channel::BLUE);

        }
    }

    // delete temporary images else they will leak the memory
    delete temp;

    return out;
}
