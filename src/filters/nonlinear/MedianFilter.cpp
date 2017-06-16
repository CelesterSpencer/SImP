#include "MedianFilter.h"

MedianFilter::MedianFilter()
{
    init("Median Filter", "non linear");

    m_filterRadius = 2;
    m_minRadius = 0;
    m_maxRadius = 10;

    addUserInput("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius);
};

void MedianFilter::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    int diameter = m_filterRadius*2 + 1;
    int median = (diameter*diameter-1)/2;

    // setup vector to store all values
    std::vector<float> valuesRed;
    std::vector<float> valuesGreen;
    std::vector<float> valuesBlue;
    valuesRed.reserve(diameter*diameter);
    valuesGreen.reserve(diameter*diameter);
    valuesBlue.reserve(diameter*diameter);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            valuesRed.clear();
            valuesGreen.clear();
            valuesBlue.clear();
            // iterate over filter
            for(int fx = -m_filterRadius; fx <= m_filterRadius; fx++)
            {
                for (int fy = -m_filterRadius; fy <= m_filterRadius; fy++)
                {
                    int posX = std::min(std::max(x+fx, 0), in->getWidth()-1);
                    int posY = std::min(std::max(y+fy, 0), in->getHeight()-1);
                    valuesRed.push_back(in->get(posX, posY, Image::Channel::RED));
                    valuesGreen.push_back(in->get(posX, posY, Image::Channel::GREEN));
                    valuesBlue.push_back(in->get(posX, posY, Image::Channel::BLUE));
                }
            }
            std::sort(valuesRed.begin(),   valuesRed.end());
            std::sort(valuesGreen.begin(), valuesGreen.end());
            std::sort(valuesBlue.begin(),  valuesBlue.end());
            out->set(valuesRed[median],   x, y, Image::Channel::RED);
            out->set(valuesGreen[median], x, y, Image::Channel::GREEN);
            out->set(valuesBlue[median],  x, y, Image::Channel::BLUE);
        }
    }

    returnImage(out);
}