#include "Saturate.h"

Saturate::Saturate()
{
    // init filter
    init("Saturate", "color adjustment");
};

void Saturate::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            float max = 0.f;
            for(int c = 0; c < std::min(in->getChannelNumber(),3); c++)
            {
                max = std::max(in->get(x,y,c),max);
            }

            for(int c = 0; c < std::min(in->getChannelNumber(),3); c++)
            {
                float val = in->get(x,y,c) / max;
                out->set(val,x,y,c);
            }
        }
    }

    returnImage(out);
}