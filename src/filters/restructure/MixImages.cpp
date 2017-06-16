#include "MixImages.h"

MixImages::MixImages()
{
    init("Mix Images", "restructure");

    m_alpha = 0.5;
    m_min = 0.0;
    m_max = 1.0;

    addUserInput("Image 1");
    addUserInput("Image 2");
    addUserInput("mix", &m_alpha, &m_min, &m_max);
};

void MixImages::process()
{
    // get only the images specified by the image selectors
    Image* in1 = getInputImage(1);
    Image* in2 = getInputImage(2);

    // create output image from bigger image dimensions and smaller color channel
    int outImageWidth = std::max(in1->getWidth(), in2->getWidth());
    int outImageHeight = std::max(in1->getHeight(), in2->getHeight());
    int outImageChannelNumber = std::min(in1->getChannelNumber(), in2->getChannelNumber());
    Image* out = new Image(outImageWidth, outImageHeight, outImageChannelNumber);

    // determine the skip to read which pixels per dimension
    float skipX1 = static_cast<float>(in1->getWidth()) / outImageWidth;
    float skipY1 = static_cast<float>(in1->getHeight()) / outImageHeight;

    float skipX2 = static_cast<float>(in2->getWidth()) / outImageWidth;
    float skipY2 = static_cast<float>(in2->getHeight()) / outImageHeight;

    // interpolate color channels depending on the defined alpha value
    out->parallel([=](Image* img, int x, int y, int c)
        {
            int x1 = static_cast<int>(std::floor(x*skipX1));
            int y1 = static_cast<int>(std::floor(y*skipY1));
            int x2 = static_cast<int>(std::floor(x*skipX2));
            int y2 = static_cast<int>(std::floor(y*skipY2));
            float val1 = in1->get(x1, y1, c);
            float val2 = in2->get(x2, y2, c);
            return (1-m_alpha)*val1+m_alpha*val2;
        });

    // return output image
    returnImage(out);
}