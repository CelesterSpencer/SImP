#ifndef SRCCMAKE_SPLITCHANNELS_H
#define SRCCMAKE_SPLITCHANNELS_H

#include "core/scene/ImageFilter.h"

class SplitChannels : public ImageFilter
{
public:
    SplitChannels();

    void process();
};


#endif //SRCCMAKE_SPLITCHANNELS_H
