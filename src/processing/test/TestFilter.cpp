#include "TestFilter.h"

void TestFilter::process()
{
    Image* in = getInputImage(0);

    std::vector<Image*> outImages;
    for (int i = 0; i < 4; i++)
    {
        Image* out = new Image;
        out->reserve(in->getWidth(), in->getHeight(), 1);
        outImages.push_back(out);
    }

    for (int i = 0; i < 4; i++)
    {
        returnImage(outImages[i]);
    }
}