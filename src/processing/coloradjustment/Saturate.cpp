#include "Saturate.h"

void Saturate::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            auto max = 0;
            for(int c = 0; c < std::min(in->getChannelNumber(),3); c++)
            {
                max = std::max(in->get(x,y,c),max);
            }

            for(int c = 0; c < std::min(in->getChannelNumber(),3); c++)
            {
                auto val = ((float)in->get(x,y,c) / max) * 255;
                out->set(val,x,y,c);
            }
        }
    }

    returnImage(out);
}