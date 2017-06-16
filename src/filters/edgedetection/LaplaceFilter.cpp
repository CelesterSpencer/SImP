#include "LaplaceFilter.h"

LaplaceFilter::LaplaceFilter()
{
    m_name = "Laplace Filter";
    m_filterGroup = "edge detection";

    static const int data[] = { 0,-1, 0,
                                -1, 4,-1,
                                0,-1, 0 };
    m_laplaceKernel = std::vector<int>(data, data + 9);
};

void LaplaceFilter::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

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
            out->set(r, x, y, Image::Channel::RED);
            out->set(g, x, y, Image::Channel::GREEN);
            out->set(b, x, y, Image::Channel::BLUE);

        }
    }

    returnImage(out);
}