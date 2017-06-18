#ifndef SRCCMAKE_SNN_H
#define SRCCMAKE_SNN_H

#include "core/scene/ImageFilter.h"

class SNN : public ImageFilter
{
public:
    SNN();

    // inherited from ImageFilter
    void process();
};


#endif //SRCCMAKE_SNN_H
