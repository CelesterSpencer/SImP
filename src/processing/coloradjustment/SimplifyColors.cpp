#include "SimplifyColors.h"

void SimplifyColors::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image;
    out->copyData(in);

    auto rNew = 0.f;
    auto gNew = 0.f;
    auto bNew = 0.f;

    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            auto r = in->get(x,y,Image::Channel::RED);
            auto g = in->get(x,y,Image::Channel::GREEN);
            auto b = in->get(x,y,Image::Channel::BLUE);
            getClosestColor(r,g,b, rNew, gNew, bNew);
            out->set(rNew,x,y,Image::Channel::RED);
            out->set(gNew,x,y,Image::Channel::GREEN);
            out->set(bNew,x,y,Image::Channel::BLUE);
        }
    }

    returnImage(out);
}

void SimplifyColors::getClosestColor(float rIn, float gIn, float bIn, float& rOut, float& gOut, float& bOut)
{
    float distance = FLT_MAX;
    auto r = 0.f;
    auto g = 0.f;
    auto b = 0.f;

    // find closest color to the incoming color
    for (int i = 0; i < m_resolution; i++)
    {
        auto h = (360.f*i)/m_resolution;
        hsvToRgb(h,1.f,1.f,r,g,b);
        auto rDiff = r-rIn;
        auto gDiff = g-gIn;
        auto bDiff = b-bIn;
        auto tempDist = sqrt(rDiff*rDiff + gDiff*gDiff + bDiff*bDiff);
        if (tempDist < distance)
        {
            rOut = r;
            gOut = g;
            bOut = b;
            distance = tempDist;
        }
    }
}

void SimplifyColors::hsvToRgb(float h, float s, float v, float& r, float& g, float& b)
{
    int hi = floor(h/60.f); // main color interval
    auto f = (h/60.f - hi); // relative position inside hi

    auto p = v*(1-s);
    auto q = v*(1-s*f);
    auto t = v*(1-s*(1-f));

    switch (hi)
    {
        case 0: case 6:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        case 5:
            r = v; g = p; b = q;
            break;
    }

    r = r*255;
    g = g*255;
    b = b*255;
}