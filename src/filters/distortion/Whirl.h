#ifndef SRCCMAKE_WHIRL_H
#define SRCCMAKE_WHIRL_H

#include "core/scene/ImageFilter.h"

class Whirl : public ImageFilter
{
public:
    Whirl();

    void process();

private:
    float m_rotationAngle;
    float m_angleMin;
    float m_angleMax;
};


#endif //SRCCMAKE_WHIRL_H
