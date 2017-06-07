#include "MixImages.h"

void MixImages::process()
{
    Image* in1 = getInputImage(1);
    Image* in2 = getInputImage(2);

    Image* outImage = new Image;

    // get the bigger image dimensions
    int outImageWidth = std::max(in1->getWidth(), in2->getWidth());
    int outImageHeight = std::max(in1->getHeight(), in2->getHeight());
    int outImageChannelNumber = std::min(in1->getChannelNumber(), in2->getChannelNumber());

    outImage->reserve(outImageWidth, outImageHeight, outImageChannelNumber);

    // determine the skip to read which pixels per dimension
    float skipX1 = (float)in1->getWidth() / outImageWidth;
    float skipY1 = (float)in1->getHeight() / outImageHeight;

    float skipX2 = (float)in2->getWidth() / outImageWidth;
    float skipY2 = (float)in2->getHeight() / outImageHeight;

    for (int y = 0; y < outImageHeight; y++)
    {
        for (int x = 0; x < outImageWidth; x++)
        {
            int x1 = std::floor(x*skipX1);
            int y1 = std::floor(y*skipY1);
            int x2 = std::floor(x*skipX2);
            int y2 = std::floor(y*skipY2);
            for (int c = 0; c < outImageChannelNumber; c++)
            {
                float val1 = in1->get(x1, y1, c);
                float val2 = in2->get(x2, y2, c);
                outImage->set((1-m_alpha)*val1+m_alpha*val2, x, y, c);
            }
        }
    }

    returnImage(outImage);
}