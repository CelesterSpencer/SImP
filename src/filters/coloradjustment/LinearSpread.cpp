#include "LinearSpread.h"

LinearSpread::LinearSpread()
{
    init("Linear Spread", "color adjustment");
};

void LinearSpread::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    float minR = 1;
    float minG = 1;
    float minB = 1;

    float maxR = 0;
    float maxG = 0;
    float maxB = 0;

    // get the min and max values
    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            minR = std::min(in->get(x,y,Image::Channel::RED),  minR);
            minB = std::min(in->get(x,y,Image::Channel::BLUE), minB);
            minG = std::min(in->get(x,y,Image::Channel::GREEN),minG);

            maxR = std::max(in->get(x,y,Image::Channel::RED),  maxR);
            maxB = std::max(in->get(x,y,Image::Channel::BLUE), maxB);
            maxG = std::max(in->get(x,y,Image::Channel::GREEN),maxG);
        }
    }

    // apply the linear spread
    for(int y = 0; y < in->getHeight(); y++)
    {
        for (int x = 0; x < in->getWidth(); x++)
        {
            float r = in->get(x,y,Image::Channel::RED);
            float g = in->get(x,y,Image::Channel::GREEN);
            float b = in->get(x,y,Image::Channel::BLUE);
            out->set((r - minR)/(maxR - minR)*1,x,y,Image::Channel::RED);
            out->set((g - minG)/(maxG - minG)*1,x,y,Image::Channel::GREEN);
            out->set((b - minB)/(maxB - minB)*1,x,y,Image::Channel::BLUE);
        }
    }

    returnImage(out);
}
