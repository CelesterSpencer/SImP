//
// Created by Windrian on 23.03.2017.
//

#include "ConvertToGrayscale.h"

Image* ConvertToGrayscale::process(Image* in)
{
    Image* out = new Image;
    out->copyData(in);

    // iterate over image
    for(int x = 0; x < in->getWidth(); x++)
    {
        for(int y = 0; y < in->getHeight(); y++)
        {
            int r = in->get(x, y, Image::Channel::RED);
            int g = in->get(x, y, Image::Channel::GREEN);
            int b = in->get(x, y, Image::Channel::BLUE);
            int gray = 0;
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
            out->set((int)gray,   x, y, Image::Channel::RGB);
        }
    }

    return out;
}