//
// Created by Windrian on 28.03.2017.
//

#include "LinearSpread.h"

Image* LinearSpread::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    int minR = 255;
    int minG = 255;
    int minB = 255;

    int maxR = 0;
    int maxG = 0;
    int maxB = 0;

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
            int r = in->get(x,y,Image::Channel::RED);
            int g = in->get(x,y,Image::Channel::GREEN);
            int b = in->get(x,y,Image::Channel::BLUE);
            out->set((float)(r - minR)/(maxR - minR)*255,x,y,Image::Channel::RED);
            out->set((float)(g - minG)/(maxG - minG)*255,x,y,Image::Channel::GREEN);
            out->set((float)(b - minB)/(maxB - minB)*255,x,y,Image::Channel::BLUE);
        }
    }

    return out;
}
