//
// Created by Windrian on 23.03.2017.
//

#include "ConvertToGrayscale.h"

void ConvertToGrayscale::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            float r = in->get(x, y, Image::Channel::RED);
            float g = in->get(x, y, Image::Channel::GREEN);
            float b = in->get(x, y, Image::Channel::BLUE);
            float gray = 0;
            switch (m_selectedOption)
            {
                case 0: // lightness
                    gray = (std::max(r, std::max(g,b)), std::min(r, std::min(g,b))) / 2;
                    break;
                case 1: // average
                    gray = (r + g + b) / 3;
                    break;
                case 2: // luminocity
                    gray = 0.21*r + 0.72*g + 0.07*b;
                    break;
            }
            out->set(gray,   x, y, Image::Channel::RGB);
        }
    }

    returnImage(out);
}