//
// Created by Windrian on 28.03.2017.
//

#include "LinearSpread.h"

void LinearSpread::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    float minR = 255;
    float minG = 255;
    float minB = 255;

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
            out->set((float)(r - minR)/(maxR - minR)*255,x,y,Image::Channel::RED);
            out->set((float)(g - minG)/(maxG - minG)*255,x,y,Image::Channel::GREEN);
            out->set((float)(b - minB)/(maxB - minB)*255,x,y,Image::Channel::BLUE);
        }
    }

    returnImage(out);
}
