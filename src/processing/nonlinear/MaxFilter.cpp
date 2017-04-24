#include "MaxFilter.h"

void MaxFilter::process()
{
    Image* in = getInputImage(0);
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
                    maxR = std::max(in->get(posX, posY, Image::Channel::RED),   maxR);
                    maxG = std::max(in->get(posX, posY, Image::Channel::GREEN), maxG);
                    maxB = std::max(in->get(posX, posY, Image::Channel::BLUE),  maxB);
                }
            }
            out->set(maxR, x, y, Image::Channel::RED);
            out->set(maxG, x, y, Image::Channel::GREEN);
            out->set(maxB, x, y, Image::Channel::BLUE);
        }
    }

    returnImage(out);
}