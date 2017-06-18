#include "SNN.h"

SNN::SNN()
{
    init("SNN", "non linear");
}

void SNN::process()
{
    // get first input image
    Image* in = getInputImage(0);

    // create output from input image
    Image* out = new Image(in);

    out->parallel([](Image* p_img, int x, int y, int c) -> float
        {
            int xl = p_img->getXInBounds(x-1);
            int xr = p_img->getXInBounds(x+1);
            int yt = p_img->getYInBounds(y-1);
            int yb = p_img->getYInBounds(y+1);
            float p1 = p_img->get(xl, yt, c);
            float p2 = p_img->get( 0, yt, c);
            float p3 = p_img->get(xr, yt, c);
            float p4 = p_img->get(xl,  0, c);
            float p5 = p_img->get( 0,  0, c);
            float p6 = p_img->get(xr,  0, c);
            float p7 = p_img->get(xl, yb, c);
            float p8 = p_img->get( 0, yb, c);
            float p9 = p_img->get(xr, yb, c);
            float val1 = (std::abs(p1-p5) < std::abs(p9-p5)) ? p1 : p9;
            float val2 = (std::abs(p2-p5) < std::abs(p8-p5)) ? p2 : p8;
            float val3 = (std::abs(p3-p5) < std::abs(p7-p5)) ? p3 : p7;
            float val4 = (std::abs(p4-p5) < std::abs(p6-p5)) ? p4 : p6;
            return (val1+val2+val3+val4) / 4.f;
        });

    returnImage(out);
}