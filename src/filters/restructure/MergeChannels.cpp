#include "MergeChannels.h"

MergeChannels::MergeChannels()
{
    m_name = "Merge Channels";
    m_filterGroup = "restructure";

    m_interactableCollection.addInteractable(new ImageSelector("R Channel"));
    m_interactableCollection.addInteractable(new ImageSelector("G Channel"));
    m_interactableCollection.addInteractable(new ImageSelector("B Channel"));
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