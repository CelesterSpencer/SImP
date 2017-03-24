//
// Created by Windrian on 22.03.2017.
//

#include "LaplaceFilter.h"

Image* LaplaceFilter::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float r = 0;
            float g = 0;
            float b = 0;
            // iterate over filter
            for(int fx = -1; fx <= 1; fx++)
            {
                for (int fy = -1; fy <= 1; fy++)
                {
                    int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                    int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                    int filterIdx = (fy+1)*3 + (fx+1);
                    r += in->get(posX, posY, Image::Channel::RED)   * m_laplaceKernel[filterIdx];
                    g += in->get(posX, posY, Image::Channel::GREEN) * m_laplaceKernel[filterIdx];
                    b += in->get(posX, posY, Image::Channel::BLUE)  * m_laplaceKernel[filterIdx];
                }
            }
            out->set((int)r, x, y, Image::Channel::RED);
            out->set((int)g, x, y, Image::Channel::GREEN);
            out->set((int)b, x, y, Image::Channel::BLUE);

        }
    }

    return out;
}