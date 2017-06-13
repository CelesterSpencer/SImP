#include "Multithreading.h"

void Multithreading::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    out->parallel([=](int w, int h, int c, float val) -> float
    {
            float min = 1;

            // iterate over filter
            for(int fx = -m_filterRadius; fx <= m_filterRadius; fx++)
            {
                for (int fy = -m_filterRadius; fy <= m_filterRadius; fy++)
                {
                    int posX = std::min(std::max(w+fx, 0), in->getWidth()-1);
                    int posY = std::min(std::max(h+fy, 0), in->getHeight()-1);
                    min = std::min(out->get(posX, posY, c), min);
                }
            }

            return min;
    }, m_threadCount);

    returnImage(out);
}