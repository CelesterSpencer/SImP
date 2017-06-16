#include "Whirl.h"

Whirl::Whirl()
{
    init("Whirl", "distortion");

    // init variables
    m_rotationAngle = 10.f;
    m_angleMin = 0.f;
    m_angleMax = 360.f;

    addUserInput("Max rotation angle", &m_rotationAngle, &m_angleMin, &m_angleMax);
}

void Whirl::process()
{
    // get the selected input image
    Image* in = getInputImage(0);

    // create output image from input
    Image* out = new Image(in);

    // determine the position of the center
    int centerX = static_cast<int>(std::round(in->getWidth()/2.f));
    int centerY = static_cast<int>(std::round(in->getHeight()/2.f));

    float maxAngleRadians = static_cast<float>(m_rotationAngle*PI / 180.f);
    float maxRadius = std::max(centerX, centerY);

    out->parallel([=](Image* img, int x, int y, int c)
    {
        // get vector from center to current pixel
        float dX = x - centerX;
        float dY = y - centerY;

        // calc rotation angle depending on distance to center
        float r = std::sqrt(dX*dX + dY*dY);
        float theta = r*maxAngleRadians / maxRadius;
        float cs = cos(theta);
        float sn = sin(theta);

        // calc rotated position
        int newX = static_cast<int>(std::round(dX*cs - dY*sn));
        int newY = static_cast<int>(std::round(dX*sn + dY*cs));
        newX = std::max(0, std::min(img->getWidth()-1, newX+centerX));
        newY = std::max(0, std::min(img->getHeight()-1, newY+centerY));

        return img->get(newX, newY, c);
    });

    returnImage(out);
}