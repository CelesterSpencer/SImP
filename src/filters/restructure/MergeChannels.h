#ifndef SRCCMAKE_MERGECHANNELS_H
#define SRCCMAKE_MERGECHANNELS_H

#include "core/scene/ImageFilter.h"

class MergeChannels : public ImageFilter
{
public:
    MergeChannels();

    void process();
};


#endif //SRCCMAKE_MERGECHANNELS_H
