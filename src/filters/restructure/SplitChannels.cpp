#include "SplitChannels.h"

SplitChannels::SplitChannels()
{
    init("Split Channels", "restructure");
};

void SplitChannels::process()
{
    Image* in = getInputImage(0);

    // create an image for every channel
    std::vector<Image*> outputImages;
    for (int i = 0; i < in->getChannelNumber(); i++)
    {
        Image* out = new Image(in->getWidth(), in->getHeight(), 1);
        outputImages.push_back(out);
    }

    // fill all images
    for (int y = 0; y < in->getHeight(); y++)
    {
        for (int x = 0; x < in->getWidth(); x++)
        {
            for (int c = 0; c < in->getChannelNumber(); c++)
            {
                outputImages[c]->set(in->get(x,y,c), x,y,Image::Channel::RED);
            }
        }
    }

    // return all images
    for (int i = 0; i < in->getChannelNumber(); i++)
    {
        returnImage(outputImages[i]);
    }
}