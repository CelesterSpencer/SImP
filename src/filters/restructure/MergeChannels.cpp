#include "MergeChannels.h"

MergeChannels::MergeChannels()
{
    init("Merge Channels", "restructure");

    // select 3 images for every channel
    addUserInput("R Channel");
    addUserInput("G Channel");
    addUserInput("B Channel");
}

void MergeChannels::process()
{
    // get input image for every channel
    Image* imageR = getInputImage(1);
    Image* imageG = getInputImage(2);
    Image* imageB = getInputImage(3);

    // create output image
    Image* out = new Image(imageR->getWidth(), imageR->getHeight(), 3);

    float gSkipX = imageG->getWidth()  / imageR->getWidth();
    float gSkipY = imageG->getHeight() / imageR->getHeight();
    float bskipX = imageB->getWidth()  / imageR->getWidth();
    float bSkipY = imageB->getHeight() / imageR->getHeight();

    out->parallel([=](Image* img, int x, int y, int c)
        {
            float val = 0;

            switch(c)
            {
                case Image::Channel::RED:
                    val = imageR->get(x, y, Image::Channel::RED);
                    break;
                case Image::Channel::GREEN:
                    val = imageG->get(x*gSkipX, y*gSkipY, Image::Channel::GREEN);
                    break;
                case Image::Channel::BLUE:
                    val = imageB->get(x*bskipX, y*bSkipY, Image::Channel::BLUE);
                    break;
            }

            return val;
        });

    returnImage(out);
}