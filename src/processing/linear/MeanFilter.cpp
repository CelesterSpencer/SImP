//
// Created by Windrian on 19.03.2017.
//

#include "MeanFilter.h"

void MeanFilter::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    int filterRadius = m_filterRadius;
    int filterSize = filterRadius*2 + 1;
    std::cout << std::to_string(filterSize) << std::endl;

    Image* temp = new Image;
    temp->copyData(in);

    // iterate over image
    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            for(int c = 0; c < in->getChannelNumber(); c++)
            {
                float val = 0;
                // iterate over filter
                for(int fx = -filterRadius; fx <= filterRadius; fx++)
                {
                    int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                    val += in->get(posX, y, c);
                }
                val = val / filterSize;
                temp->set((int)val, x, y, c);
            }
        }
    }

    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            for(int c = 0; c < in->getChannelNumber(); c++)
            {
                float val = 0;
                // iterate over filter
                for (int fy = -filterRadius; fy <= filterRadius; fy++)
                {
                    int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                    val += temp->get(x, posY, c);
                }
                val = val / filterSize;
                out->set((int)val, x, y, c);
            }
        }
    }

    // delete temporary images else they will leak the memory
    delete temp;

    returnImage(out);
}
