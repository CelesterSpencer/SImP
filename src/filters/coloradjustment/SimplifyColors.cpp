#include "SimplifyColors.h"

SimplifyColors::SimplifyColors()
{
    m_name = "Simplify colors";
    m_filterGroup = "color adjustment";

    m_resolution = 3;
    m_minResolution = 3;
    m_maxResolution = 30;

    m_interactableCollection.addInteractable(
            new ISlider("Number of colors", &m_resolution, &m_minResolution, &m_maxResolution)
    );
};

void SimplifyColors::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    float rNew = 0.f;
    float gNew = 0.f;
    float bNew = 0.f;

    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            float r = in->get(x,y,Image::Channel::RED);
            float g = in->get(x,y,Image::Channel::GREEN);
            float b = in->get(x,y,Image::Channel::BLUE);
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
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;

    // find closest color to the incoming color
    for (int i = 0; i < m_resolution; i++)
    {
        float h = (360.f*i)/m_resolution;
        hsvToRgb(h,1.f,1.f,r,g,b);
        float rDiff = r-rIn;
        float gDiff = g-gIn;
        float bDiff = b-bIn;
        float tempDist = sqrt(rDiff*rDiff + gDiff*gDiff + bDiff*bDiff);
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
    float f = (h/60.f - hi); // relative position inside hi

    float p = v*(1-s);
    float q = v*(1-s*f);
    float t = v*(1-s*(1-f));

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
}