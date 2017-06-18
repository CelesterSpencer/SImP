#ifndef SRCCMAKE_SATURATE_H
#define SRCCMAKE_SATURATE_H

#include "core/scene/ImageFilter.h"

class Saturate : public ImageFilter
{
public:
    Saturate();

    void process();
};


#endif //SRCCMAKE_SATURATE_H
